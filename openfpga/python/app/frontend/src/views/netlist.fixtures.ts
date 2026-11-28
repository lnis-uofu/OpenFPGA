// A small design for the tests of the netlist views: its atoms export and its clusters export
import { type ViewData, makeView } from "./format";

/** in0 -> x1 (u_a) -> x2, x3 (u_a.u_b) -> q (u_c) -> out; x3 is guessed */
export function atoms(): ViewData {
  // pins: atom, dir (0 in, 1 out), net
  const pins: [number, number, number][] = [
    [0, 1, 0], // in0 drives n_in
    [1, 0, 0],
    [1, 1, 1], // x1: n_in -> n1
    [2, 0, 1],
    [2, 1, 2], // x2: n1 -> n2
    [3, 0, 1],
    [3, 0, 2],
    [3, 1, 3], // x3: n1, n2 -> n3
    [4, 0, 3],
    [4, 1, 4], // q: n3 -> n4
    [5, 0, 4], // out
  ];
  const pinStart = [0, 1, 3, 5, 8, 10];
  return makeView("atoms", {
    models: { name: [".input", ".names", ".latch", ".output"] },
    modules: { name: ["", "u_a", "u_a.u_b", "u_c"] },
    atoms: {
      name: ["in0", "u_a.x1", "u_a.u_b.x2", "$abc$7$x3", "u_c.q", "out:out"],
      model: Uint16Array.of(0, 1, 1, 1, 2, 3),
      module: Uint32Array.of(0, 1, 2, 2, 3, 0),
      how: Uint8Array.of(3, 0, 0, 2, 0, 3),
      cluster: Int32Array.of(0, 1, 1, 1, 2, 3),
      pin_start: Uint32Array.from(pinStart),
      pin_count: Uint32Array.of(1, 2, 2, 3, 2, 1),
    },
    pins: {
      atom: Uint32Array.from(pins, (p) => p[0]),
      port: pins.map(() => "p"),
      bit: new Uint16Array(pins.length),
      dir: Uint8Array.from(pins, (p) => p[1]),
      net: Uint32Array.from(pins, (p) => p[2]),
    },
    nets: {
      name: ["n_in", "n1", "n2", "n3", "n4"],
      driver: Int32Array.of(0, 2, 4, 7, 9),
      pins: Uint32Array.of(2, 3, 2, 2, 2),
      inside: Uint32Array.of(2, 3, 2, 2, 2),
      // The ids of the nets of the atoms: not their rows
      id: Uint32Array.of(4, 3, 2, 1, 0),
    },
  });
}

export function clusters(): ViewData {
  return makeView("clusters", {
    block_types: { name: ["EMPTY", "io", "clb"] },
    blocks: {
      name: ["in0", "clb_a", "clb_q", "out:out"],
      id: Uint32Array.of(0, 1, 2, 3),
      type: Uint16Array.of(1, 2, 2, 1),
    },
    nets: { name: ["n_in", "n3", "n4"], net: Uint32Array.of(0, 1, 2) },
  });
}
