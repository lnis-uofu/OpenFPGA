// deck.gl and Canvas2D must draw the same polylines: the middle of the last segment of each path
// has to be red in both (deck.gl once drew only as many segments as there were paths).
//   npm run e2e:parity      (without DISPLAY, or WebGL does not start in headless Chromium)
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import assert from "node:assert/strict";

const port = 5800 + Math.floor(Math.random() * 100);
const server = spawn(
  "npx",
  ["vite", "--host", "127.0.0.1", "--port", String(port), "--strictPort"],
  { stdio: ["ignore", "pipe", "pipe"] },
);
await new Promise((resolve, reject) => {
  server.stdout.on("data", (d) => String(d).includes("Local") && resolve());
  setTimeout(() => reject(new Error("vite did not start")), 30000);
});
const browser = await chromium.launch({
  args: ["--enable-unsafe-swiftshader", "--use-angle=swiftshader"],
});
let failed = false;
try {
  const page = await browser.newPage({ viewport: { width: 840, height: 320 } });
  const errors = [];
  page.on("pageerror", (e) => errors.push(String(e)));
  await page.goto(`http://127.0.0.1:${port}/parity.html`);
  await page.waitForFunction(() => window.parityReady === true, null, {
    timeout: 30000,
  });
  await page.waitForTimeout(1500);
  assert.deepEqual(errors, []);
  const probes = await page.evaluate(() => window.parityProbes);
  for (const id of ["deck", "canvas2d"]) {
    const shot = await page.locator(`#${id}`).screenshot();
    const red = await page.evaluate(
      async ([data, spots]) => {
        const image = new Image();
        image.src = `data:image/png;base64,${data}`;
        await image.decode();
        const canvas = document.createElement("canvas");
        canvas.width = image.width;
        canvas.height = image.height;
        const ctx = canvas.getContext("2d");
        ctx.drawImage(image, 0, 0);
        const scale = image.width / 400;
        // Red in a box of 7 pixels around the spot
        return spots.map(([x, y]) => {
          const d = ctx.getImageData(
            Math.round(x * scale) - 3,
            Math.round(y * scale) - 3,
            7,
            7,
          ).data;
          for (let i = 0; i < d.length; i += 4)
            if (d[i] > 200 && d[i + 1] < 80 && d[i + 2] < 80) return true;
          return false;
        });
      },
      [shot.toString("base64"), probes],
    );
    console.log(id, JSON.stringify(red));
    try {
      assert.deepEqual(
        red,
        probes.map(() => true),
        `${id}: a path is not drawn to its end`,
      );
    } catch (e) {
      failed = true;
      console.error(String(e.message));
    }
  }
} finally {
  await browser.close();
  server.kill();
}
if (failed) process.exit(1);
console.log("parity: ok");
