// A check with a real model in the browser: not part of the tests (a model is slow and does not
// always do the same). The page is used as a person would, and the result is printed.
//   MODEL_URL=http://127.0.0.1:8081/v1 MODEL_NAME=qwen2.5-7b [TOOL_MODE=json] [SHOT=file.png] npm run e2e:model
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import { mkdtempSync, mkdirSync, copyFileSync } from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";

const here = new URL(".", import.meta.url).pathname;
const repo = path.resolve(here, "../../../../..");
const flow = path.join(repo, "openfpga_flow");
const moduleDir = process.env.OPENFPGA_PYTHON_PATH ?? path.join(repo, "build", "openfpga");
const python = process.env.PYTHON ?? "python3";
const port = 8700 + Math.floor(Math.random() * 200);
const root = mkdtempSync(path.join(tmpdir(), "openfpga-app-"));
const dirs = [];
for (const [folder, file] of [
  ["vpr_arch", path.join(flow, "vpr_arch", "k6_frac_N10_tileable_40nm.xml")],
  ["openfpga_arch", path.join(flow, "openfpga_arch", "k6_frac_N10_40nm_openfpga.xml")],
  ["routing_test", path.join(flow, "benchmarks", "ubm", "interface", "routing_test", "routing_test.blif")],
]) {
  const dir = path.join(root, "inputs", folder);
  mkdirSync(dir, { recursive: true });
  copyFileSync(file, path.join(dir, path.basename(file)));
  dirs.push(dir);
}
const server = spawn(
  python,
  [
    "-m", "openfpga_app", "--root", path.join(root, "ws"), "--port", String(port),
    "--model-url", process.env.MODEL_URL, "--model-name", process.env.MODEL_NAME ?? "local",
    "--tool-mode", process.env.TOOL_MODE ?? "native", "--record-dir", path.join(root, "records"),
    ...dirs.flatMap((d) => ["--designs-dir", d]),
  ],
  { env: { ...process.env, PYTHONPATH: [moduleDir, path.resolve(here, "../.."), path.resolve(here, "../../../mcp")].join(path.delimiter) } },
);
let output = "";
server.stdout.on("data", (d) => (output += d));
server.stderr.on("data", (d) => (output += d));
process.on("exit", () => server.kill());
const url = await (async () => {
  for (let i = 0; i < 200; i++) {
    const m = output.match(/http:\/\/\S+\?token=\S+/);
    if (m) return m[0];
    await new Promise((r) => setTimeout(r, 100));
  }
  throw new Error("the app did not start: " + output);
})();
const browser = await chromium.launch();
try {
  const page = await browser.newPage({ viewport: { width: 1400, height: 900 } });
  await page.goto(url);
  const started = Date.now();
  await page.fill("textarea", process.env.MESSAGE ?? "Run the flow on routing_test with a channel width of 20 and tell me the Fmax.");
  await page.keyboard.press("Enter");
  await page.waitForFunction(() => document.querySelector("button[type=submit]"), null, { timeout: 600000 });
  console.log("seconds:", ((Date.now() - started) / 1000).toFixed(0));
  console.log("cards:", await page.locator(".card").count(), "ctx:", await page.locator(".context").textContent().catch(() => "-"));
  console.log("assistant:", (await page.locator(".message-assistant .text").allTextContents()).join(" | ").slice(-500));
  console.log("top bar:", await page.locator("header .latest").textContent());
  console.log("records:", path.join(root, "records"));
  if (process.env.SHOT) await page.screenshot({ path: process.env.SHOT });
} finally {
  await browser.close();
  server.kill();
}
