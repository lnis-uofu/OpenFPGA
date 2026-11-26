// The views of the design over a real session (docs/plan_design_views.md, phases 3 and 4): the
// floorplan, the pins and the routing of a 4x4 and of a 32x32 device, the selection that they share,
// the route of a net against the routing export, the choice of the renderer, a lost WebGL context,
// and the same picture from deck.gl and from Canvas2D.
//   PYTHON=python3 npm run e2e:views      (SHOTS=<folder> keeps the screenshots)
import { chromium } from "playwright";
import { spawn } from "node:child_process";
import { createHash } from "node:crypto";
import {
  cpSync,
  mkdirSync,
  mkdtempSync,
  readFileSync,
  readdirSync,
  statSync,
  writeFileSync,
} from "node:fs";
import { tmpdir } from "node:os";
import path from "node:path";
import { inflateSync } from "node:zlib";
import assert from "node:assert/strict";

const here = new URL(".", import.meta.url).pathname;
const repo = path.resolve(here, "../../../../..");
const flow = path.join(repo, "openfpga_flow");
const moduleDir =
  process.env.OPENFPGA_PYTHON_PATH ?? path.join(repo, "build", "openfpga");
const python = process.env.PYTHON ?? "python3";
const port = 8700 + Math.floor(Math.random() * 200);
const root = mkdtempSync(path.join(tmpdir(), "openfpga-views-"));
const shots =
  process.env.SHOTS ??
  mkdtempSync(path.join(tmpdir(), "openfpga-views-shots-"));
mkdirSync(shots, { recursive: true });

// The packages: the 4x4 one of the repository, and a 32x32 copy of it
const packages = path.join(root, "..", path.basename(root) + "-packages");
const small = path.join(packages, "k6_frac_n10_2x2");
cpSync(path.join(flow, "device-packages", "k6_frac_n10_2x2"), small, {
  recursive: true,
});
const big = path.join(packages, "k6_frac_n10_32x32_test");
cpSync(small, big, { recursive: true });
{
  const arch = path.join(big, "arch", "vpr_arch.xml");
  const text = readFileSync(arch, "utf8");
  const layout = text.match(
    / *<fixed_layout name="2x2"[\s\S]*?<\/fixed_layout>\n/,
  )[0];
  writeFileSync(
    arch,
    text.replace(
      layout,
      layout +
        layout.replace(
          'name="2x2" width="4" height="4"',
          'name="32x32" width="34" height="34"',
        ),
    ),
  );
  const manifestPath = path.join(big, "device_package.json");
  const manifest = JSON.parse(readFileSync(manifestPath, "utf8"));
  manifest.name = "k6_frac_n10_32x32_test";
  manifest.device.grid = { width: 32, height: 32 };
  if (manifest.flow_defaults?.vpr_options?.device)
    manifest.flow_defaults.vpr_options.device = "32x32";
  delete manifest.reference;
  writeFileSync(manifestPath, JSON.stringify(manifest, null, 2));
  const files = [];
  const walk = (dir) => {
    for (const name of readdirSync(dir)) {
      const full = path.join(dir, name);
      if (statSync(full).isDirectory()) walk(full);
      else if (name !== "CHECKSUMS.sha256") files.push(full);
    }
  };
  walk(big);
  files.sort();
  writeFileSync(
    path.join(big, "CHECKSUMS.sha256"),
    files
      .map(
        (f) =>
          `${createHash("sha256").update(readFileSync(f)).digest("hex")}  ${path.relative(big, f).split(path.sep).join("/")}`,
      )
      .join("\n") + "\n",
  );
}

const pythonPath = [
  moduleDir,
  path.resolve(here, "../.."),
  path.resolve(here, "../../../mcp"),
].join(path.delimiter);
const server = spawn(
  python,
  [
    "-m",
    "openfpga_app",
    "--root",
    root,
    "--console",
    "--port",
    String(port),
    "--device-packages",
    packages,
    "--designs-dir",
    path.join(flow, "benchmarks", "ubm", "interface", "routing_test"),
  ],
  { env: { ...process.env, PYTHONPATH: pythonPath } },
);
let output = "";
server.stdout.on("data", (d) => (output += d));
server.stderr.on("data", (d) => (output += d));
const stop = () => server.kill();
process.on("exit", stop);
async function started() {
  for (let i = 0; i < 300; i++) {
    const match = output.match(/http:\/\/\S+\?token=\S+/);
    if (match) return match[0];
    await new Promise((r) => setTimeout(r, 100));
  }
  throw new Error("the app did not start: " + output);
}

/** The tables of a file of a view, read in Node (the format of src/views/format.ts) */
function readView(buffer) {
  const length = buffer.readUInt32LE(8);
  const header = JSON.parse(buffer.subarray(12, 12 + length).toString("utf8"));
  const start = Math.ceil((12 + length) / 8) * 8;
  const tables = {};
  for (const table of header.tables) {
    const columns = {};
    for (const c of table.columns) {
      const at = start + c.offset;
      if (c.dtype === "str") {
        const ends = Array.from({ length: c.count }, (_, i) =>
          buffer.readUInt32LE(at + 4 * i),
        );
        const blob = start + c.blob_offset;
        columns[c.name] = ends
          .slice(0, -1)
          .map((e, i) =>
            buffer.subarray(blob + e, blob + ends[i + 1]).toString("utf8"),
          );
      } else {
        const read = {
          u8: [1, "readUInt8"],
          u16: [2, "readUInt16LE"],
          u32: [4, "readUInt32LE"],
          i32: [4, "readInt32LE"],
          f32: [4, "readFloatLE"],
          i8: [1, "readInt8"],
          i16: [2, "readInt16LE"],
          f64: [8, "readDoubleLE"],
        }[c.dtype];
        columns[c.name] = Array.from({ length: c.count }, (_, i) =>
          buffer[read[1]](at + read[0] * i),
        );
      }
    }
    tables[table.name] = columns;
  }
  return { meta: header.meta, tables };
}

/** The pixels of a PNG of a screenshot (8-bit RGB or RGBA, not interlaced) */
function pixels(png) {
  let at = 8;
  let width = 0;
  let height = 0;
  let channels = 4;
  const data = [];
  while (at < png.length) {
    const length = png.readUInt32BE(at);
    const type = png.toString("ascii", at + 4, at + 8);
    const body = png.subarray(at + 8, at + 8 + length);
    if (type === "IHDR") {
      width = body.readUInt32BE(0);
      height = body.readUInt32BE(4);
      channels = body[9] === 6 ? 4 : 3;
    } else if (type === "IDAT") data.push(body);
    at += 12 + length;
  }
  const raw = inflateSync(Buffer.concat(data));
  const stride = width * channels;
  const out = Buffer.alloc(height * stride);
  for (let y = 0; y < height; y++) {
    const filter = raw[y * (stride + 1)];
    for (let x = 0; x < stride; x++) {
      const v = raw[y * (stride + 1) + 1 + x];
      const a = x >= channels ? out[y * stride + x - channels] : 0;
      const b = y > 0 ? out[(y - 1) * stride + x] : 0;
      const c =
        x >= channels && y > 0 ? out[(y - 1) * stride + x - channels] : 0;
      const p = a + b - c;
      const pred = [
        0,
        a,
        b,
        (a + b) >> 1,
        Math.abs(p - a) <= Math.abs(p - b) && Math.abs(p - a) <= Math.abs(p - c)
          ? a
          : Math.abs(p - b) <= Math.abs(p - c)
            ? b
            : c,
      ][filter];
      out[y * stride + x] = (v + pred) & 255;
    }
  }
  return { width, height, channels, data: out };
}

/** The mean colour of the cells of a grid over a picture */
function cells(image, cols = 24, rows = 16) {
  const out = [];
  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      const sum = [0, 0, 0];
      let n = 0;
      for (
        let y = Math.floor((r * image.height) / rows);
        y < Math.floor(((r + 1) * image.height) / rows);
        y++
      ) {
        for (
          let x = Math.floor((c * image.width) / cols);
          x < Math.floor(((c + 1) * image.width) / cols);
          x++
        ) {
          const i = (y * image.width + x) * image.channels;
          sum[0] += image.data[i];
          sum[1] += image.data[i + 1];
          sum[2] += image.data[i + 2];
          n++;
        }
      }
      out.push(sum.map((s) => s / Math.max(1, n)));
    }
  }
  return out;
}

const url = await started();
// Headless Chromium draws WebGL with SwiftShader: the software path
const browser = await chromium.launch({
  args: ["--enable-unsafe-swiftshader", "--use-angle=swiftshader"],
});
try {
  const context = await browser.newContext({
    viewport: { width: 1400, height: 900 },
  });
  const page = await context.newPage();
  const errors = [];
  page.on("pageerror", (e) => errors.push(String(e)));
  page.on(
    "console",
    (m) =>
      m.type() === "error" &&
      !/WebGL|GL_|GPU|Failed to load resource/.test(m.text()) &&
      errors.push(m.text()),
  );
  // A view asks for data that a stage has not made yet (the placement of a new session): the core
  // refuses it (400) and the view shows what it has. Any other failed request is an error
  const refused = [];
  page.on("response", (r) => {
    if (r.status() < 400) return;
    const where = new URL(r.url()).pathname;
    if (r.status() === 400 && (where === "/api/view" || where === "/api/tile"))
      refused.push(r.url());
    else errors.push(`${r.status()} ${r.url()}`);
  });
  await page.goto(url);
  const say = async (line) => {
    const chat = page.locator('[role=tab]:has-text("Chat")');
    if (await chat.count()) await chat.click();
    await page.fill("textarea", line);
    await page.keyboard.press("Enter");
    await page.waitForFunction(
      () => !document.querySelector(".card-running"),
      null,
      { timeout: 120000 },
    );
    await page.waitForFunction(
      () => document.querySelector("button[type=submit]"),
      null,
      { timeout: 120000 },
    );
  };
  const viewReady = (name, layer) =>
    page.waitForFunction(
      ([n, l]) =>
        window.openfpgaViews?.[n]?.scene() &&
        window.openfpgaViews[n].drawn(l) > 0,
      [name, layer],
      { timeout: 60000 },
    );
  const shot = (name) =>
    page.locator(".view-host").screenshot({ path: path.join(shots, name) });

  // A routed session of the 4x4 device
  await page.click("text=Create session from device package…");
  await page.click(
    ".package:has-text('k6_frac_n10_2x2') >> text=Create session",
  );
  await page.waitForSelector(".stage-reached:has-text('DEVICE')", {
    timeout: 60000,
  });
  await say("/read_inputs circuit=routing_test.blif");
  await say("/run_stage stage=route");
  await page.waitForSelector("header >> text=ROUTED ✔", { timeout: 120000 });

  // Auto finds the software WebGL and draws with Canvas2D (P4.1: a forced software GL)
  await page.click('[aria-label="Open the placement"]');
  await viewReady("floorplan", "blocks");
  assert.equal(
    await page.evaluate(() => window.openfpgaViews.floorplan.backend()),
    "canvas2d",
    "auto: software WebGL goes to Canvas2D",
  );
  assert.ok(await page.isVisible(".view-backend-now:has-text('Canvas2D')"));
  assert.ok(await page.isVisible("[role=tab]:has-text('Placement')"));
  await page.waitForSelector(".view-legend li:has-text('Tiles')");
  assert.ok(await page.isVisible(".view-legend li:has-text('Blocks')"));
  await shot("floorplan-4x4.png");
  // Folded, the legend leaves the drawing to the clicks (and stays folded in the other views)
  await page.click(".legend-fold");
  assert.equal(await page.locator(".view-legend li").count(), 0);

  // A block of the floorplan, clicked, is the selection; an I/O is selected in the pins too
  const io = await page.evaluate(() => {
    const scene = window.openfpgaViews.floorplan.scene();
    const blocks = scene.layers.find((l) => l.id === "blocks");
    for (let i = 0; i < blocks.x.length; i++)
      if (blocks.name(i).startsWith("out:")) return blocks.name(i);
    return blocks.name(0);
  });
  const at = await page.evaluate(
    (name) => window.openfpgaViews.floorplan.screenOf({ kind: "block", name }),
    io,
  );
  const box = await page.locator(".view-host").boundingBox();
  await page.mouse.click(box.x + at[0], box.y + at[1]);
  await page.waitForSelector(`.view-selected:has-text('${io}')`);
  await page.waitForFunction(
    () =>
      window.openfpgaViews.floorplan
        .scene()
        .layers.some((l) => l.id === "selection:blocks"),
    null,
    { timeout: 10000 },
  );
  await page.click('[aria-label="Open the floorplan"]');
  await viewReady("pins", "ports");
  assert.ok(await page.isVisible("[role=tab]:has-text('Floorplan')"));
  await page.waitForFunction(
    () => window.openfpgaViews.pins.drawn("selection") > 0,
    null,
    { timeout: 30000 },
  );
  await page.waitForSelector(".view-selected:has-text('slot')");
  await shot("pins-4x4.png");

  // The routing: the route of a net, drawn, is the route of the routing export (P4.2)
  await page.click('[aria-label="Open the routing"]');
  await viewReady("routing", "wires");
  await shot("routing-4x4.png");
  const net = await page.evaluate(() =>
    window.openfpgaViews.routing
      .scene()
      .layers.find((l) => l.id === "wires")
      .name(0),
  );
  const drawn = await page.evaluate((n) => {
    const wires = window.openfpgaViews.routing
      .scene()
      .layers.find((l) => l.id === "wires");
    let count = 0;
    for (let i = 0; i < wires.xy.length / 4; i++)
      if (wires.name(i) === n) count++;
    return count;
  }, net);
  const routing = readView(
    await (
      await page.request.get(new URL(`/api/view?kind=routing`, url).href)
    ).body(),
  );
  const r = routing.tables.nets.name.indexOf(net);
  const first = routing.tables.nets.node_start[r];
  const types = routing.tables.nodes.type.slice(
    first,
    first + routing.tables.nets.node_count[r],
  );
  assert.equal(
    drawn,
    types.filter((t) => t === 4 || t === 5).length,
    `the wires of ${net} are those of its route`,
  );
  // Selected, it is drawn over everything; the floorplan shows the box of the net
  const wireAt = await page.evaluate(() => {
    const v = window.openfpgaViews.routing.view();
    const wires = window.openfpgaViews.routing
      .scene()
      .layers.find((l) => l.id === "wires");
    const [x1, y1, x2, y2] = wires.xy.slice(0, 4);
    return [
      v.width / 2 + ((x1 + x2) / 2 - v.cx) * v.scale,
      v.height / 2 - ((y1 + y2) / 2 - v.cy) * v.scale,
    ];
  });
  const rbox = await page.locator(".view-host").boundingBox();
  await page.mouse.click(rbox.x + wireAt[0], rbox.y + wireAt[1]);
  await page.waitForFunction(
    () => window.openfpgaViews.routing.drawn("selection:nets") > 0,
    null,
    { timeout: 30000 },
  );
  await shot("routing-4x4-selected.png");

  // The renderers: WebGL chosen draws with deck.gl; the same picture as Canvas2D (P4.3)
  await page.locator(".view-host").hover({ position: { x: 5, y: 5 } });
  await page.mouse.move(rbox.x + 2, rbox.y + 2);
  await page.keyboard.press("Escape");
  await page.waitForTimeout(300);
  const canvasShot = pixels(await page.locator(".view-host").screenshot());
  await page.selectOption('[aria-label="Renderer"]', "webgl");
  await page.waitForFunction(
    () => window.openfpgaViews.routing?.backend() === "deck",
    null,
    { timeout: 30000 },
  );
  await page.waitForTimeout(1500);
  await shot("routing-4x4-deck.png");
  const deckShot = pixels(await page.locator(".view-host").screenshot());
  const a = cells(canvasShot);
  const b = cells(deckShot);
  const diff =
    a.reduce(
      (sum, cell, i) =>
        sum +
        Math.abs(cell[0] - b[i][0]) +
        Math.abs(cell[1] - b[i][1]) +
        Math.abs(cell[2] - b[i][2]),
      0,
    ) /
    (a.length * 3);
  console.log(
    `parity of the routing (deck.gl vs Canvas2D): mean difference ${diff.toFixed(2)} of 255 a channel`,
  );
  assert.ok(
    diff < 12,
    `deck.gl and Canvas2D draw the same picture (mean difference ${diff.toFixed(2)})`,
  );
  // The floorplan as well
  await page.click('[role=tab]:has-text("Placement")');
  await viewReady("floorplan", "blocks");
  await page.waitForTimeout(1500);
  const deckFloor = pixels(await page.locator(".view-host").screenshot());
  await page.selectOption('[aria-label="Renderer"]', "canvas2d");
  await page.waitForFunction(
    () => window.openfpgaViews.floorplan?.backend() === "canvas2d",
    null,
    { timeout: 30000 },
  );
  await page.waitForTimeout(300);
  const canvasFloor = pixels(await page.locator(".view-host").screenshot());
  const fa = cells(canvasFloor);
  const fb = cells(deckFloor);
  const floorDiff =
    fa.reduce(
      (sum, cell, i) =>
        sum +
        Math.abs(cell[0] - fb[i][0]) +
        Math.abs(cell[1] - fb[i][1]) +
        Math.abs(cell[2] - fb[i][2]),
      0,
    ) /
    (fa.length * 3);
  console.log(
    `parity of the floorplan (deck.gl vs Canvas2D): mean difference ${floorDiff.toFixed(2)}`,
  );
  assert.ok(
    floorDiff < 12,
    `the floorplans are the same picture (mean difference ${floorDiff.toFixed(2)})`,
  );

  // A lost WebGL context: Canvas2D at once
  await page.selectOption('[aria-label="Renderer"]', "webgl");
  await page.waitForFunction(
    () => window.openfpgaViews.floorplan?.backend() === "deck",
    null,
    { timeout: 30000 },
  );
  await page.waitForTimeout(1000);
  await page.evaluate(() =>
    document
      .querySelector(".view-host canvas")
      ?.getContext("webgl2")
      ?.getExtension("WEBGL_lose_context")
      ?.loseContext(),
  );
  await page.waitForFunction(
    () => window.openfpgaViews.floorplan?.backend() === "canvas2d",
    null,
    { timeout: 30000 },
  );
  assert.ok(await page.isVisible(".view-backend-now:has-text('Canvas2D')"));
  await page.selectOption('[aria-label="Renderer"]', "auto");

  // The 32x32 device: the objects close up, the coarse tiles far out
  await page.click(".session-action:text-is('Close')");
  await page.click("text=Create session from device package…");
  await page.click(
    ".package:has-text('k6_frac_n10_32x32_test') >> text=Create session",
  );
  await page.waitForSelector(".stage-reached:has-text('DEVICE')", {
    timeout: 120000,
  });
  await say("/read_inputs circuit=routing_test.blif");
  await say("/run_stage stage=route");
  await page.waitForSelector("header >> text=ROUTED ✔", { timeout: 300000 });
  await page.click('[aria-label="Open the placement"]');
  await viewReady("floorplan", "blocks");
  await shot("floorplan-32x32.png");
  // Out of the zoom of the objects (6 pixels a tile), into the coarse tiles
  await page.locator(".view-host").focus();
  for (let i = 0; i < 4; i++) await page.keyboard.press("-");
  await page.waitForFunction(
    () =>
      window.openfpgaViews.floorplan
        .scene()
        .layers.some((l) => l.kind === "grid"),
    null,
    { timeout: 60000 },
  );
  await page.waitForSelector(".view-status:has-text('level')");
  await shot("floorplan-32x32-coarse.png");
  await page.click('[aria-label="Open the routing"]');
  await page.waitForFunction(
    () => window.openfpgaViews?.routing?.scene(),
    null,
    { timeout: 60000 },
  );
  await page.locator(".view-host").focus();
  for (let i = 0; i < 3; i++) await page.keyboard.press("-");
  await page.waitForFunction(
    () =>
      window.openfpgaViews.routing
        .scene()
        .layers.some((l) => l.kind === "grid"),
    null,
    { timeout: 60000 },
  );
  await shot("routing-32x32-congestion.png");
  await page.click('[aria-label="Open the floorplan"]');
  await viewReady("pins", "ports");
  await shot("pins-32x32.png");

  assert.deepEqual(errors, []);
  console.log(
    "refused by the core (a stage not reached yet):",
    refused.map((u) => new URL(u).search),
  );
  console.log(`screenshots in ${shots}`);
  console.log("e2e views: ok");
} finally {
  await browser.close();
  stop();
}
