// An end-to-end check of the app over a real session, with no model: the page is used as a person
// would, a flow is run from the chat and the console, and the sidebar follows.
//   OPENFPGA_PYTHON_PATH=<dir of openfpga.py> PYTHON=python3 npm run e2e:session
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import { existsSync, mkdtempSync, readFileSync } from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";
import assert from "node:assert/strict";

const here = new URL(".", import.meta.url).pathname;
const repo = path.resolve(here, "../../../../..");
const flow = path.join(repo, "openfpga_flow");
const moduleDir = process.env.OPENFPGA_PYTHON_PATH ?? path.join(repo, "build", "openfpga");
const python = process.env.PYTHON ?? "python3";
const port = 8700 + Math.floor(Math.random() * 200);
const root = mkdtempSync(path.join(tmpdir(), "openfpga-app-"));
const files = {
  vpr: "k6_frac_N10_tileable_40nm.xml",
  arch: "k6_frac_N10_40nm_openfpga.xml",
  circuit: "routing_test.blif",
};
const inputs = [
  path.join(flow, "vpr_arch"),
  path.join(flow, "openfpga_arch"),
  path.join(flow, "benchmarks", "ubm", "interface", "routing_test"),
];
// INSTALLED=1: the packages are the installed ones (a clean environment), not the source tree
const pythonPath = process.env.INSTALLED
  ? ""
  : [moduleDir, ...(process.env.APP_PYTHONPATH ? [process.env.APP_PYTHONPATH] : [path.resolve(here, "../.."), path.resolve(here, "../../../mcp")])].join(path.delimiter);
const server = spawn(
  python,
  ["-m", "openfpga_app", "--root", root, "--console", "--port", String(port), "--device-packages", path.join(flow, "device-packages"), ...inputs.flatMap((d) => ["--designs-dir", d])],
  {
    env: { ...process.env, PYTHONPATH: pythonPath },
  },
);
let output = "";
server.stdout.on("data", (d) => (output += d));
server.stderr.on("data", (d) => (output += d));
const stop = () => server.kill();
process.on("exit", stop);

async function started() {
  for (let i = 0; i < 200; i++) {
    const match = output.match(/http:\/\/\S+\?token=\S+/);
    if (match) return match[0];
    await new Promise((r) => setTimeout(r, 100));
  }
  throw new Error("the app did not start: " + output);
}

const url = await started();
const browser = await chromium.launch();
try {
  const page = await browser.newPage({ viewport: { width: 1400, height: 850 } });
  const errors = [];
  page.on("console", (m) => m.type() === "error" && errors.push(m.text()));
  await page.goto(url);
  const say = async (line) => {
    await page.fill("textarea", line);
    await page.keyboard.press("Enter");
    await page.waitForFunction(() => !document.querySelector(".card-running"), null, { timeout: 60000 });
    await page.waitForFunction(() => document.querySelector("button[type=submit]"), null, { timeout: 60000 });
  };

  // The first screen: a prompt in the middle and the two ways to start a session, nothing else
  await page.waitForSelector("text=No model is connected");
  await page.waitForSelector('[aria-label="Start"] textarea');
  assert.ok(await page.isVisible("text=Create session from device package…"));
  assert.equal(await page.locator('[aria-label="Session and design files"]').count(), 0, "no sidebar yet");
  if (process.env.SHOT_WELCOME) await page.screenshot({ path: process.env.SHOT_WELCOME });
  // What is asked from it is answered in a small window below the prompt, and the screen stays
  await page.fill('[aria-label="Start"] textarea', "/help");
  await page.keyboard.press("Enter");
  await page.waitForSelector('[aria-label="Answer"]:has-text("session_open")', { timeout: 30000 });
  assert.equal(await page.locator('[aria-label="Session and design files"]').count(), 0, "still the first screen");
  if (process.env.SHOT_WELCOME2) await page.screenshot({ path: process.env.SHOT_WELCOME2 });
  // A session is opened from there: when the answer is complete, the full interface comes
  await page.fill('[aria-label="Start"] textarea', "/session_open");
  await page.keyboard.press("Enter");
  await page.waitForSelector("text=worker running", { timeout: 30000 });
  assert.equal(await page.locator('[aria-label="Start"]').count(), 0, "the welcome screen is gone");
  assert.ok(await page.isVisible("strong:has-text('s1')"));

  // A file from the person's own disk goes into the session, and the tools read it by its name
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "mine.blif",
    mimeType: "application/octet-stream",
    buffer: readFileSync(path.join(inputs[2], files.circuit)),
  });
  await page.waitForSelector(".upload-row:has-text('mine.blif'):has-text('circuit')");
  assert.ok(existsSync(path.join(root, "s1", "uploads", "mine.blif")), "written in the session directory");
  // The file opens in a tab, in Monaco
  await page.click(".upload-row .file:has-text('mine.blif')");
  await page.waitForSelector(".file-view .monaco-editor", { timeout: 30000 });
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("model"), null, { timeout: 15000 });
  // The circuit is coloured as BLIF: the directives, the cubes and the names are not one colour
  await page.waitForFunction(
    () => new Set([...document.querySelectorAll(".file-view .view-lines span[class^='mtk']")].map((e) => e.className)).size >= 3,
    null,
    { timeout: 15000 },
  ); // the tokens come a moment after the text
  if (process.env.SHOT_BLIF) await page.screenshot({ path: process.env.SHOT_BLIF });
  assert.ok(await page.isVisible('[role=tab]:has-text("mine.blif")'));
  // The file of the session is edited in the editor and saved in place (Ctrl+S)
  assert.ok(await page.isDisabled(".save-file"), "nothing to save yet");
  await page.click(".file-view .monaco-editor .view-lines");
  await page.keyboard.press("Control+Home");
  await page.keyboard.type("# edited by hand\n"); // a comment at the start: VPR reads none after .end
  await page.waitForSelector(".save-file:not([disabled])");
  await page.keyboard.press("Control+s");
  await page.waitForSelector(".file-note-ok:has-text('Saved.')", { timeout: 15000 });
  assert.ok(readFileSync(path.join(root, "s1", "uploads", "mine.blif"), "utf8").includes("# edited by hand"), "saved on the disk");
  assert.ok(await page.isDisabled(".save-file"), "saved: nothing more to save");
  await page.click('[role=tab]:has-text("Chat")');
  await say(`/read_inputs vpr_arch=${files.vpr} layout=2x2 openfpga_arch=${files.arch} circuit=mine.blif`);
  await page.waitForSelector(".file-row .file:has-text('mine.blif')", { timeout: 30000 }); // read as an input

  // A file that is taken out of the session
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "gone.blif",
    mimeType: "application/octet-stream",
    buffer: readFileSync(path.join(inputs[2], files.circuit)),
  });
  await page.waitForSelector(".upload-row:has-text('gone.blif')");
  page.once("dialog", (dialog) => dialog.accept());
  await page.click('[aria-label="Remove gone.blif"]');
  await page.waitForSelector(".upload-row:has-text('gone.blif')", { state: "detached" });
  assert.ok(!existsSync(path.join(root, "s1", "uploads", "gone.blif")), "deleted from the session directory");

  // A flow from the chat lines
  await say("/set_option name=general.clock_modeling value=route");
  await say("/set_option name=router.route_chan_width value=20");
  await say(`/read_inputs vpr_arch=${files.vpr} layout=2x2 openfpga_arch=${files.arch} circuit=${files.circuit}`);
  await page.waitForSelector("text=layout 2x2", { timeout: 30000 });
  for (const stage of ["pack", "place", "route", "analysis"]) await say(`/run_stage stage=${stage}`);
  await page.waitForSelector("text=ANALYZED ✔", { timeout: 30000 });
  await page.waitForSelector("text=routing_test.route", { timeout: 30000 });

  // A result opens in a tab
  await say("/get_timing paths=1");
  await page.click('button.card-head:has-text("get_timing")');
  await page.click("text=Open in tab");
  await page.waitForSelector("text=timing summary", { timeout: 30000 });
  if (process.env.SHOT_RESULT) await page.screenshot({ path: process.env.SHOT_RESULT });
  await page.click('[role=tab]:has-text("Chat")');

  // The console runs the shell in the same session; the sidebar follows
  await page.click("text=▸ Console");
  await page.fill('[aria-label="Command"]', "set_app_option --name place.seed --value 3");
  await page.keyboard.press("Enter");
  await page.waitForSelector("text=Options changed (");
  await page.click("text=Options changed (");
  await page.waitForSelector("text=place.seed = 3", { timeout: 30000 });
  await page.fill('[aria-label="Command"]', "report_session");
  await page.keyboard.press("Enter");
  await page.waitForSelector(".line-output");
  await page.waitForSelector("text=model › run_stage", { timeout: 30000 });

  // The console is made higher by dragging its top edge, and lower again with the keys
  const before = await page.locator(".console-lines").evaluate((e) => e.getBoundingClientRect().height);
  const edge = await page.locator(".console-resize").boundingBox();
  await page.mouse.move(edge.x + edge.width / 2, edge.y + edge.height / 2);
  await page.mouse.down();
  await page.mouse.move(edge.x + edge.width / 2, edge.y - 120, { steps: 6 });
  await page.mouse.up();
  const after = await page.locator(".console-lines").evaluate((e) => e.getBoundingClientRect().height);
  assert.ok(after > before + 80, `the console is higher: ${before} -> ${after}`);
  await page.focus(".console-resize");
  await page.keyboard.press("ArrowDown");
  const lower = await page.locator(".console-lines").evaluate((e) => e.getBoundingClientRect().height);
  assert.ok(lower < after, "the arrow lowers it");
  await page.dblclick(".console-resize");
  assert.equal(Math.round(await page.locator(".console-lines").evaluate((e) => e.getBoundingClientRect().height)), 180);

  // The seed dropped the stages after the packing; the last checkpoint has them all
  assert.ok(await page.isVisible("header >> text=PACKED ✔"));
  await page.click("text=restore");
  await page.waitForSelector("header >> text=ANALYZED ✔");
  await page.click("text=close");
  await page.waitForSelector("text=Create session from device package…"); // back at the welcome screen

  // The closed session is saved: it opens again from its last checkpoint, with its stages
  await page.click("text=Open session…");
  await page.waitForSelector(".session-entry:has-text('saved')", { timeout: 15000 });
  await page.click(".session-entry button:has-text('Open')");
  await page.waitForSelector("header >> text=ANALYZED ✔", { timeout: 60000 });
  await page.click("text=close");
  await page.waitForSelector("text=Create session from device package…");

  // A session from a device package: the architectures are read, and it waits for the circuit
  await page.click("text=Create session from device package…");
  await page.waitForSelector(".package:has-text('k6_frac_n10_2x2')");
  if (process.env.SHOT_PICKER) await page.screenshot({ path: process.env.SHOT_PICKER });
  await page.click(".package:has-text('k6_frac_n10_2x2') >> text=Create session");
  await page.waitForSelector("text=Waiting for a circuit");
  assert.ok(await page.isVisible("text=device k6_frac_n10_2x2 · 2x2 · K6"));
  await page.waitForSelector("text=layout 2x2", { timeout: 30000 });
  assert.equal(await page.locator(".file-row .file-label:has-text('circuit')").count(), 0, "no circuit yet");
  assert.ok(!(await page.isVisible("header >> text=DEVICE ✔")), "nothing reached");
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "design.blif",
    mimeType: "application/octet-stream",
    buffer: readFileSync(path.join(inputs[2], files.circuit)),
  });
  await page.waitForSelector(".upload-row:has-text('design.blif')");
  // The session has its architectures and no circuit: the upload is read as the circuit, by itself
  await page.waitForSelector(".file-row .file:has-text('design.blif')", { timeout: 30000 });
  assert.equal(await page.locator("text=Waiting for a circuit").count(), 0);
  await say("/run_stage stage=pack");
  await page.waitForSelector("header >> text=PACKED ✔", { timeout: 60000 });
  // A result of the flow and a file of the package open in the viewer
  await page.click(".results .file:has-text('.net')");
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("block"), null, { timeout: 30000 });
  await page.click(".file-row .file[title$='arch/vpr_arch.xml']");
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("architecture"), null, { timeout: 30000 });

  assert.deepEqual(errors, []);
  if (process.env.SHOT) await page.screenshot({ path: process.env.SHOT });
  console.log("e2e session: ok");
} finally {
  await browser.close();
  stop();
}
