// An end-to-end check in a real (headless) browser: the app is started with the recorded demo, and
// the page is used as a person would. Needs a built front end and `npx playwright install chromium`.
//   PYTHON=python3 npm run e2e          (PYTHONPATH must find openfpga_app: the app/ directory)
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import assert from "node:assert/strict";

const python = process.env.PYTHON ?? "python3";
const port = 8700 + Math.floor(Math.random() * 200);
const server = spawn(python, ["-m", "openfpga_app", "--demo", "--console", "--port", String(port)], {
  env: { ...process.env, PYTHONPATH: process.env.PYTHONPATH ?? new URL("../..", import.meta.url).pathname },
});
let output = "";
server.stdout.on("data", (d) => (output += d));
server.stderr.on("data", (d) => (output += d));
const stop = () => server.kill();
process.on("exit", stop);

async function started() {
  for (let i = 0; i < 100; i++) {
    const match = output.match(/http:\/\/\S+\?token=\S+/);
    if (match) return match[0];
    await new Promise((r) => setTimeout(r, 100));
  }
  throw new Error("the app did not start: " + output);
}

const url = await started();
const browser = await chromium.launch();
try {
  // Without the token, the page says what to do
  const stranger = await browser.newPage();
  const refused = await stranger.goto(`http://localhost:${port}/`);
  assert.equal(refused.status(), 401);
  assert.match(await stranger.textContent("body"), /printed in the terminal/);

  const page = await browser.newPage({ viewport: { width: 1400, height: 850 } });
  const errors = [];
  page.on("console", (m) => m.type() === "error" && errors.push(m.text()));
  await page.goto(url);
  // The token is not left in the address
  assert.equal(new URL(page.url()).search, "");
  // A new session has every section and nothing in it; no tab bar, no results area
  assert.ok(await page.isVisible("text=No session"));
  assert.equal(await page.locator(".centre [role=tablist]").count(), 0);

  await page.fill("textarea", "Run the flow on routing_test and tell me the Fmax.");
  await page.keyboard.press("Enter");
  await page.waitForSelector("text=recorded demo", { timeout: 60000 });
  if (process.env.SHOT_CHAT) await page.screenshot({ path: process.env.SHOT_CHAT });
  // The sidebar followed the conversation
  assert.ok(await page.isVisible("text=chan width 24"));
  assert.ok(await page.isVisible(".stage-reached:has-text('DEVICE')"));
  // Calls in a row fold; the failed one is a card of its own
  assert.ok(await page.isVisible("text=5 calls"));
  assert.ok(await page.isVisible("div.card-failed"));

  // A result opens in a tab next to Chat, which cannot be closed
  await page.click('button.card-head:has-text("get_timing")');
  await page.click("text=Open in tab");
  assert.equal(await page.locator("[role=tab]").first().textContent(), "Chat");
  assert.equal(await page.locator('[aria-label="Close Chat"]').count(), 0);
  assert.ok(await page.isVisible("text=Work in progress"));
  await page.click("text=⫴ split");
  assert.ok(await page.isVisible('[aria-label="Result of get_timing"]'));
  await page.click('[aria-label="Close get_timing"]');
  assert.equal(await page.locator(".centre [role=tablist]").count(), 0);

  // The console runs a command
  await page.click("text=▸ Console");
  await page.fill('[aria-label="Command"]', "help");
  await page.keyboard.press("Enter");
  await page.waitForSelector("text=Demo console");
  assert.ok(await page.isVisible("text=model › run_stage"));

  // The theme: the system's while it is auto; a choice wins over the system and is kept
  const background = () => page.evaluate(() => getComputedStyle(document.body).backgroundColor);
  await page.emulateMedia({ colorScheme: "dark" });
  assert.equal(await background(), "rgb(20, 22, 26)");
  await page.click('[aria-label="Theme: auto"]'); // light
  assert.equal(await page.getAttribute("html", "data-theme"), "light");
  assert.equal(await background(), "rgb(255, 255, 255)");
  await page.reload();
  assert.equal(await page.getAttribute("html", "data-theme"), "light", "the choice is kept");
  await page.click('[aria-label="Theme: light"]'); // dark
  await page.emulateMedia({ colorScheme: "light" });
  assert.equal(await background(), "rgb(20, 22, 26)");
  await page.click('[aria-label="Theme: dark"]'); // auto again
  assert.equal(await page.getAttribute("html", "data-theme"), null);
  assert.equal(await background(), "rgb(255, 255, 255)");

  assert.deepEqual(errors, []);
  if (process.env.SHOT) await page.screenshot({ path: process.env.SHOT });
  console.log("e2e: ok");
} finally {
  await browser.close();
  stop();
}
