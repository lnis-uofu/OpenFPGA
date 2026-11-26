// An end-to-end check of the app over a real session, with no model: the page is used as a person
// would, a flow is run from the chat and the console, and the sidebar follows.
//   OPENFPGA_PYTHON_PATH=<dir of openfpga.py> PYTHON=python3 npm run e2e:session
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import { existsSync, mkdirSync, mkdtempSync, readFileSync, writeFileSync } from "node:fs";
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
// A design directory with a hierarchy, for the hierarchy tab
const designs = mkdtempSync(path.join(tmpdir(), "openfpga-designs-"));
mkdirSync(path.join(designs, "hier", "rtl"), { recursive: true });
writeFileSync(
  path.join(designs, "hier", "rtl", "top.v"),
  [
    "module leaf(input a, input b, output y);",
    "  assign y = a ^ b;",
    "endmodule",
    "module sub #(parameter W = 4) (input [W-1:0] a, output y);",
    "  wire t;",
    "  leaf u_leaf0 (.a(a[0]), .b(a[1]), .y(t));",
    "  leaf u_leaf1 (.a(t), .b(a[W-1]), .y(y));",
    "endmodule",
    "module top(input [7:0] a, output [1:0] y);",
    "  sub #(.W(8)) u_wide (.a(a), .y(y[0]));",
    "  sub u_narrow (.a(a[3:0]), .y(y[1]));",
    "endmodule",
    "",
  ].join("\n"),
);
writeFileSync(
  path.join(designs, "hier", "design.json"),
  JSON.stringify({ format_version: "1.0", name: "hier", license: "MIT", top: "top", files: [{ role: "rtl", path: "rtl/top.v" }] }),
);
const inputs = [
  path.join(flow, "vpr_arch"),
  path.join(flow, "openfpga_arch"),
  path.join(flow, "benchmarks", "ubm", "interface", "routing_test"),
  designs,
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
  // The design files of the sidebar start folded: unfold them to see the files
  async function unfoldFiles() {
    const fold = page.locator(".sidebar .section-fold:has-text('Design files')");
    await fold.waitFor();
    if ((await fold.getAttribute("aria-expanded")) === "false") await fold.click();
  }
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
  assert.ok(await page.isVisible(".session-tab[aria-selected=true]:has-text('s1')"));

  // A file from the person's own disk goes into the session, and the tools read it by its name
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "mine.blif",
    mimeType: "application/octet-stream",
    buffer: readFileSync(path.join(inputs[2], files.circuit)),
  });
  await unfoldFiles();
  await page.waitForSelector(".upload-row:has-text('mine.blif')");
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
  await page.waitForSelector(".upload-row.loaded .file:has-text('mine.blif')", { timeout: 30000 }); // read as the circuit
  // The design is a netlist: synthesis is not needed, and is greyed out; the architecture is a button
  await page.waitForSelector(".stage-skipped:has-text('SYNTHESIS')");
  assert.equal(await page.locator(".file-row .file-label:has-text('vpr arch')").count(), 0, "the architecture files are not listed");
  assert.ok(await page.isVisible('[aria-label="Open the architecture files"]'));
  if (process.env.SHOT_SIDEBAR_BLIF) await page.screenshot({ path: process.env.SHOT_SIDEBAR_BLIF });

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
  await page.waitForSelector(".stage-reached:has-text('DEVICE')", { timeout: 30000 });

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

  // The bitstream: a stage of its own, its file in the results, downloaded
  await say("/run_stage stage=bitstream");
  await page.waitForSelector("header >> text=BITSTREAM ✔", { timeout: 60000 });
  await page.waitForSelector(".result-download:has-text('bitstream/routing_test.bit')", { timeout: 30000 });
  const href = await page.getAttribute(".result-download a.download", "href");
  const download = await page.request.get(new URL(href, page.url()).href);
  assert.equal(download.status(), 200, "the bitstream is downloaded");
  assert.ok((await download.body()).length > 1000, "with its bits");
  if (process.env.SHOT_BITSTREAM) await page.screenshot({ path: process.env.SHOT_BITSTREAM });

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

  // The mode Tcl: a script of lines in the interpreter of the session; a query is a dict
  await page.click('.console-mode button:text-is("Tcl")');
  await page.fill('[aria-label="Tcl script"]', 'set seed [dict get [lindex [dict get [report_app_option --name place.seed --json] options] 0] value]\nputs "seed is $seed"\nexpr {$seed * 14}');
  await page.keyboard.press("Enter");
  await page.waitForSelector(".line-output:text-is('42')", { timeout: 30000 });
  await page.waitForSelector("text=seed is 3");
  assert.ok(await page.isVisible("text=tcl% set seed"), "the script is shown with the prompt of Tcl");
  await page.click('.console-mode button:text-is("Shell")');
  await page.waitForSelector('[aria-label="Command"]');

  // The seed dropped the stages after the packing; the last checkpoint has them all
  assert.ok(await page.isVisible("header >> text=PACKED ✔"));
  await page.click(".checkpoint-status"); // the checkpoints are folded
  await page.click("text=restore");
  await page.waitForSelector("header >> text=ANALYZED ✔");
  await page.click(".session-action:text-is('Close')");
  await page.waitForSelector("text=Create session from device package…"); // back at the welcome screen

  // The closed session is saved: it opens again from its last checkpoint, with its stages
  await page.click("text=Open session…");
  await page.waitForSelector(".session-entry:has-text('saved')", { timeout: 15000 });
  await page.click(".session-entry button:has-text('Open')");
  await page.waitForSelector("header >> text=ANALYZED ✔", { timeout: 60000 });

  // The constraints of the design in an ODC: the sidebar shows them, and a check opens its line,
  // which the editor marks
  await page.click('[role=tab]:has-text("Chat")');
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "top.odc",
    mimeType: "text/plain",
    buffer: Buffer.from("odc_version 1\ncreate_clock -period 10 clk\nset_input_delay 0 -clock clk [get_ports {IN0 INX}]\n"),
  });
  await unfoldFiles();
  await page.waitForSelector(".upload-row:has-text('top.odc')");
  await say("/set_constraint kind=odc file=top.odc");
  await page.waitForSelector(".odc-head:has-text('2 commands')", { timeout: 30000 });
  assert.ok(await page.isVisible(".odc-head .odc-warning:has-text('1')"), "one warning");
  assert.ok(await page.isVisible(".odc-line:has-text('clocks clk')"));
  await page.click("button.fold:has-text('Checks (1)')");
  await page.click(".odc-checks .file:has-text('top.odc:3')");
  await page.waitForSelector(".file-view .monaco-editor", { timeout: 30000 });
  await page.waitForSelector(".file-view .squiggly-warning", { timeout: 15000 });
  await page.waitForSelector(".file-view .file-note-warn:has-text('1 warning')");
  if (process.env.SHOT_ODC) await page.screenshot({ path: process.env.SHOT_ODC });
  await page.click(".session-action:text-is('Close')");
  await page.waitForSelector("text=Create session from device package…");

  // A session from a device package: the architectures are read, and it waits for the circuit
  await page.click("text=Create session from device package…");
  await page.waitForSelector(".package:has-text('k6_frac_n10_2x2')");
  if (process.env.SHOT_PICKER) await page.screenshot({ path: process.env.SHOT_PICKER });
  await page.click(".package:has-text('k6_frac_n10_2x2') >> text=Create session");
  await page.waitForSelector("text=Waiting for a circuit");
  assert.ok(await page.isVisible(".stage-device:has-text('k6_frac_n10_2x2')"));
  await page.waitForSelector(".stage-reached:has-text('DEVICE')", { timeout: 30000 });

  await page.click('[role=tab]:has-text("Chat")');
  assert.equal(await page.locator(".upload-row.loaded").count(), 0, "no circuit yet");
  await unfoldFiles(); // a new sidebar: folded again
  assert.ok(!(await page.isVisible("header >> text=DEVICE ✔")), "nothing reached");
  await page.setInputFiles('[aria-label="Upload files"]', {
    name: "design.blif",
    mimeType: "application/octet-stream",
    buffer: readFileSync(path.join(inputs[2], files.circuit)),
  });
  await page.waitForSelector(".upload-row:has-text('design.blif')");
  // The session has its architectures and no circuit: the upload is read as the circuit, by itself
  await page.waitForSelector(".upload-row.loaded .file:has-text('design.blif')", { timeout: 30000 });
  assert.equal(await page.locator("text=Waiting for a circuit").count(), 0);
  await say("/run_stage stage=pack");
  await page.waitForSelector("header >> text=PACKED ✔", { timeout: 60000 });
  // A result of the flow and a file of the package open in the viewer
  await page.click(".results .file:has-text('.net')");
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("block"), null, { timeout: 30000 });
  await page.click('[aria-label="Open the architecture files"]'); // opens the two architecture files in tabs
  await page.waitForSelector('[role=tab]:has-text("vpr_arch.xml")');
  await page.click('[role=tab]:has-text("vpr_arch.xml")');
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("architecture"), null, { timeout: 30000 });

  await page.click('[role=tab]:has-text("Chat")');
  // Verilog is synthesized for the package of the session: the netlist is the circuit, and the files show
  await say('/write_file name=inv.v content="module inv(input a, output y);\n  assign y = ~a;\nendmodule\n"');
  await say("/synthesize top=inv files=inv.v");
  await page.waitForSelector(".upload-row .file:has-text('synth/inv.blif')", { timeout: 60000 });
  await page.waitForSelector(".upload-row.loaded .file:has-text('synth/inv.blif')", { timeout: 30000 }); // read as the circuit
  await page.waitForSelector(".stage-reached:has-text('SYNTHESIS')"); // synthesis is a stage that was reached
  if (process.env.SHOT_SIDEBAR) await page.screenshot({ path: process.env.SHOT_SIDEBAR });
  await page.click(".upload-row .file:has-text('synth/inv.blif')");
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("inv"), null, { timeout: 30000 });

  // The menu opens the device packages and the designs in tabs, each with a search
  await page.click(".menu-item:has-text('Devices')");
  await page.waitForSelector(".panel-item:has-text('k6_frac_n10_2x2') >> text=this session");
  await page.fill('[aria-label="Search the device packages"]', "no such device");
  await page.waitForSelector("text=No device package matches");
  await page.click(".menu-item:has-text('Designs')");
  await page.waitForSelector('[aria-label="Design directories"]');
  assert.ok(await page.isVisible('[role=tab]:has-text("Devices")'), "both tabs stay");
  // The sidebar is made wider by dragging its edge, and keeps the width
  const narrow = (await page.locator(".sidebar").boundingBox()).width;
  const edge2 = await page.locator(".sidebar-resize").boundingBox();
  await page.mouse.move(edge2.x + edge2.width / 2, edge2.y + 200);
  await page.mouse.down();
  await page.mouse.move(edge2.x + 120, edge2.y + 200, { steps: 6 });
  await page.mouse.up();
  const wide = (await page.locator(".sidebar").boundingBox()).width;
  assert.ok(wide > narrow + 80, `the sidebar is wider: ${narrow} -> ${wide}`);
  assert.equal(await page.evaluate(() => localStorage.getItem("openfpga-sidebar-width")), String(Math.round(wide)));
  await page.dblclick(".sidebar-resize");

  // The hierarchy of a design that is added: a separate run of Yosys, which does not change the
  // flow; the line of an instance opens in the viewer
  await page.click(".menu-item:has-text('Designs')");
  await page.click(".panel-item:has-text('hier') >> text=Add to session");
  // Added, the tab of the designs closes; open again, it marks the design
  await page.waitForSelector("[role=tab]:has-text('Designs')", { state: "detached", timeout: 30000 });
  await page.click(".menu-item:has-text('Designs')");
  await page.waitForSelector(".panel-item:has-text('hier') >> text=in this session", { timeout: 30000 });
  // The sidebar elaborates it by itself, and shows its first level
  await page.waitForSelector(".sidebar .hier-compact .hier-instance:has-text('u_wide')", { timeout: 60000 });
  await page.waitForSelector(".sidebar .hier-side-head:has-text('7 instances, 4 modules')");
  await page.click(".sidebar [aria-label='Expand u_narrow']");
  await page.waitForSelector(".sidebar .hier-compact .hier-instance:has-text('u_leaf1')");
  // The design files are below it, and fold
  await page.click(".sidebar .section-fold:has-text('Design files')");
  await page.waitForSelector(".sidebar .upload-row", { state: "detached" });
  await page.click(".sidebar .section-fold:has-text('Design files')");
  await page.waitForSelector(".sidebar .upload-row:has-text('rtl/top.v')");
  await page.click(".sidebar [aria-label='Open the hierarchy in a tab']");
  await page.waitForSelector(".hier-instance:has-text('u_wide')", { timeout: 60000 });
  await page.waitForSelector("text=#(W=8)");
  await page.waitForSelector(".hier-instance:has-text('u_leaf1')");
  await page.click("[aria-label='Collapse u_wide']");
  await page.click(".hier-mode >> text=Modules");
  await page.waitForSelector(".hier-modules td:has-text('W=8')");
  await page.click(".hier-mode >> text=Tree");
  await page.click("button[title='Open design/hier/rtl/top.v at line 10']");
  await page.waitForFunction(() => document.querySelector(".file-view .view-lines")?.textContent?.includes("u_wide"), null, { timeout: 30000 });

  assert.deepEqual(errors, []);
  if (process.env.SHOT) await page.screenshot({ path: process.env.SHOT });
  console.log("e2e session: ok");
} finally {
  await browser.close();
  stop();
}
