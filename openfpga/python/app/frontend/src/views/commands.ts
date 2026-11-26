// What the model shows in the views (docs/plan_design_views.md, 3.6 and P7.1): the tool show of the
// MCP server answers a command, the app sends it to the page as the event view_command, and this
// runs it with the commands of the views. A view of the object is opened if none is.
import type { ViewCommandEvent } from "../api";
import {
  type Panel,
  consoleLines,
  openPanel,
  setViewCommandHandler,
  tabs,
} from "../store";
import { wholeView } from "./data";
import { showInNetlist } from "./netlist";
import type { ObjectKind } from "./scene";
import { runViewCommand, selectRefs } from "./selection";
import { clusterNames, loadTiming, pathRefs, pathsOf } from "./timing";

/** The views by the names that the model gives them (the names of their tabs) */
export const VIEW_PANELS: Record<string, Panel> = {
  floorplan: "pins",
  placement: "floorplan",
  routing: "routing",
  timing: "timing",
  netlist: "netlist",
  clusters: "clusters",
};
const VIEWS = new Set<string>(Object.values(VIEW_PANELS));

/** The view that shows a kind of object best */
const HOME: Record<string, Panel> = {
  block: "floorplan",
  net: "routing",
  atom: "netlist",
  module: "netlist",
  path: "timing",
};

/** A view that shows the object: the one open if there is one, else the one for its kind */
function ensureView(kind: string) {
  if (tabs.value.some((t) => t.panel && VIEWS.has(t.panel))) return;
  const home = HOME[kind];
  if (home) openPanel(home);
}

/** A path of the timing by its index: its refs, and the atoms on it */
async function path(index: number) {
  const [timing, placement] = await Promise.all([
    loadTiming(),
    wholeView("placement"),
  ]);
  const found = pathsOf(timing)[index];
  if (!found) throw new Error(`The timing has no path ${index + 1}`);
  return {
    refs: pathRefs(found, clusterNames(placement)),
    atoms: found.points.map((p) => p.atom).filter(Boolean),
  };
}

export async function runModelCommand(
  command: ViewCommandEvent,
): Promise<void> {
  const kind = (command.kind ?? "") as ObjectKind;
  const name = command.name ?? "";
  switch (command.action) {
    case "clear":
      runViewCommand({ op: "clear" });
      return;
    case "open_view": {
      const panel = VIEW_PANELS[command.view ?? ""];
      if (panel) runViewCommand({ op: "open_view", view: panel as never });
      return;
    }
    case "highlight":
      ensureView(kind);
      runViewCommand({ op: "highlight", kind, name });
      return;
    case "select":
    case "zoom_to":
      ensureView(kind);
      if (kind === "path") {
        const found = await path(Number(name));
        selectRefs(found.refs, `path ${Number(name) + 1}`);
      } else if (command.action === "select")
        runViewCommand({ op: "select", kind, name });
      runViewCommand({ op: "zoom_to", kind, name });
      return;
    case "show_cone":
      showInNetlist({ cone: name, what: `the cone of ${name}` });
      runViewCommand({ op: "select", kind: "atom", name });
      openPanel("netlist");
      runViewCommand({ op: "zoom_to", kind: "atom", name });
      return;
    case "show_path": {
      const found = await path(Number(name));
      showInNetlist({ atoms: found.atoms, what: `path ${Number(name) + 1}` });
      selectRefs(found.refs, `path ${Number(name) + 1}`);
      openPanel("netlist");
      runViewCommand({ op: "zoom_to", kind: "path", name });
      return;
    }
  }
}

setViewCommandHandler((command) => {
  runModelCommand(command).catch((error) => {
    consoleLines.value = [
      ...consoleLines.value,
      {
        kind: "model",
        text: `view › ${command.action} failed: ${error?.message ?? error}`,
      },
    ];
  });
});
