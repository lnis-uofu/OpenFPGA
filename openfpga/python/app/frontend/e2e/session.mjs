// An end-to-end check of the app over a real session, with no model: the page is used as a person
// would, a flow is run from the chat and the console, and the sidebar follows.
//   OPENFPGA_PYTHON_PATH=<dir of openfpga.py> PYTHON=python3 npm run e2e:session
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import { mkdtempSync } from "node:fs";
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

  // No model: the chat says how to call a tool
  await page.waitForSelector("text=No model is connected");
  // A session is opened from the sidebar
  await page.click("text=+ new");
  await page.waitForSelector("text=worker running");
  assert.ok(await page.isVisible("strong:has-text('s1')"));

  // A flow from the chat lines
  await say("/set_option name=general.clock_modeling value=route");
  await say("/set_option name=router.route_chan_width value=20");
  await say(`/read_inputs vpr_arch=${files.vpr} layout=2x2 openfpga_arch=${files.arch} circuit=${files.circuit}`);
  assert.ok(await page.isVisible("text=layout 2x2"));
  for (const stage of ["pack", "place", "route", "analysis"]) await say(`/run_stage stage=${stage}`);
  assert.ok(await page.isVisible("text=ANALYZED ✔"));
  assert.ok(await page.isVisible("text=routing_test.route"));

  // A result opens in a tab
  await say("/get_timing paths=1");
  await page.click('button.card-head:has-text("get_timing")');
  await page.click("text=Open in tab");
  assert.ok(await page.isVisible("text=timing_summary"));
  await page.click('[role=tab]:has-text("Chat")');

  // The console runs the shell in the same session; the sidebar follows
  await page.click("text=▸ Console");
  await page.fill('[aria-label="Command"]', "set_app_option --name place.seed --value 3");
  await page.keyboard.press("Enter");
  await page.waitForSelector("text=Options changed (");
  await page.click("text=Options changed (");
  assert.ok(await page.isVisible("text=place.seed = 3"));
  await page.fill('[aria-label="Command"]', "report_session");
  await page.keyboard.press("Enter");
  await page.waitForSelector(".line-output");
  assert.ok(await page.isVisible("text=model › run_stage"));

  // The seed dropped the stages after the packing; the last checkpoint has them all
  assert.ok(await page.isVisible("header >> text=PACKED ✔"));
  await page.click("text=restore");
  await page.waitForSelector("header >> text=ANALYZED ✔");
  await page.click("text=close");
  await page.waitForSelector("text=(no session)");

  assert.deepEqual(errors, []);
  if (process.env.SHOT) await page.screenshot({ path: process.env.SHOT });
  console.log("e2e session: ok");
} finally {
  await browser.close();
  stop();
}
