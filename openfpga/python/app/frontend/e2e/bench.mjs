// The renderer benchmark (src/bench/bench.ts) in headless Chromium: a table of frame times for each
// scene on deck.gl and on Canvas2D. Headless Chromium draws WebGL with SwiftShader, a software
// renderer: the slow path of a remote desktop.   npm run bench
import { chromium } from "playwright";
import { spawn } from "node:child_process";

const port = 5300 + Math.floor(Math.random() * 200);
const server = spawn(
  "npx",
  ["vite", "--host", "127.0.0.1", "--port", String(port), "--strictPort"],
  { stdio: ["ignore", "pipe", "pipe"] },
);
const stop = () => server.kill();
process.on("exit", stop);
await new Promise((resolve, reject) => {
  server.stdout.on("data", (d) => String(d).includes("Local") && resolve());
  setTimeout(() => reject(new Error("vite did not start")), 30000);
});
const browser = await chromium.launch({
  args: ["--enable-unsafe-swiftshader", "--use-angle=swiftshader"],
});
const scenes = [
  "floorplan-32",
  "routing-32",
  "floorplan-128",
  "routing-128-300k",
  "tracks-1m",
  "heat-500",
];
const only = process.env.SCENE ? [process.env.SCENE] : scenes;
const limit = Number(process.env.LIMIT_S ?? 180) * 1000;
const results = [];
try {
  const page = await browser.newPage({
    viewport: { width: 1600, height: 1000 },
  });
  await page.goto(`http://127.0.0.1:${port}/bench.html`);
  console.log("WebGL:", await page.evaluate(() => window.benchRenderer));
  // One pair a page: a pair that is too slow is cut, and the others still run
  for (const scene of only) {
    for (const backend of ["deck", "canvas2d"]) {
      const errors = [];
      const tab = await browser.newPage({
        viewport: { width: 1600, height: 1000 },
      });
      tab.on("pageerror", (e) => errors.push(String(e)));
      try {
        await tab.goto(
          `http://127.0.0.1:${port}/bench.html?auto=1&scene=${scene}&backend=${backend}`,
        );
        await tab.waitForFunction(() => window.benchDone === true, null, {
          timeout: limit,
        });
        const [result] = await tab.evaluate(() => window.benchResults);
        results.push(result);
        console.log(JSON.stringify(result));
      } catch {
        results.push({ scene, backend, timeout_s: limit / 1000 });
        console.log(
          JSON.stringify({ scene, backend, timeout_s: limit / 1000 }),
        );
      }
      if (errors.length) console.log("errors:", errors);
      await tab.close();
    }
  }
  console.log("RESULT " + JSON.stringify(results));
} finally {
  await browser.close();
  stop();
}
