// The selection that all the views share (docs/plan_design_views.md, 3.3 and 3.6), and the commands
// that drive the views: plain data, run from the views, from the console and, later, from the model.
import { effect, signal } from "@preact/signals";
import { consoleLines, openPanel, type Panel, view } from "../store";
import { type ObjectKind, type ObjectRef, refKey } from "./scene";

/** The objects that are selected, in every view */
export const selection = signal<ObjectRef[]>([]);
/** The objects that are shown brighter (a hover in another view, a command) */
export const highlighted = signal<ObjectRef[]>([]);
/** A request to show an object: each view that has it zooms to it */
export const zoomRequest = signal<{ ref: ObjectRef; seq: number } | null>(null);

export const isSelected = (ref: ObjectRef) =>
  selection.value.some((r) => refKey(r) === refKey(ref));

export type ViewCommand =
  | { op: "select"; kind: ObjectKind; name: string; add?: boolean }
  | { op: "highlight"; kind: ObjectKind; name: string }
  | { op: "clear" }
  | { op: "zoom_to"; kind: ObjectKind; name: string }
  | {
      op: "open_view";
      view:
        "floorplan" | "pins" | "routing" | "timing" | "netlist" | "clusters";
    };

const VIEWS: Panel[] = [
  "floorplan",
  "pins",
  "routing",
  "timing",
  "netlist",
  "clusters",
];
let seq = 0;

/** Run a command of the views. It is logged in the console; returns what it did */
export function runViewCommand(command: ViewCommand): string {
  let text: string;
  switch (command.op) {
    case "select": {
      const ref = { kind: command.kind, name: command.name };
      const others = command.add
        ? selection.value.filter((r) => refKey(r) !== refKey(ref))
        : [];
      selection.value = [...others, ref];
      text = `select ${command.kind} ${command.name}`;
      break;
    }
    case "highlight":
      highlighted.value = [{ kind: command.kind, name: command.name }];
      text = `highlight ${command.kind} ${command.name}`;
      break;
    case "clear":
      selection.value = [];
      highlighted.value = [];
      text = "clear";
      break;
    case "zoom_to":
      zoomRequest.value = {
        ref: { kind: command.kind, name: command.name },
        seq: ++seq,
      };
      text = `zoom_to ${command.kind} ${command.name}`;
      break;
    case "open_view":
      if (!VIEWS.includes(command.view))
        return `There is no view ${command.view}`;
      openPanel(command.view);
      text = `open_view ${command.view}`;
      break;
  }
  consoleLines.value = [
    ...consoleLines.value,
    { kind: "model", text: `view › ${text}` },
  ];
  return text;
}

/** Select several objects at once (a path: the path, its blocks, its nets), logged as what */
export function selectRefs(refs: ObjectRef[], what: string) {
  selection.value = refs;
  highlighted.value = [];
  consoleLines.value = [
    ...consoleLines.value,
    { kind: "model", text: `view › select ${what}` },
  ];
}

// Another session: what was selected is of the one before
let sessionId: string | null | undefined;
effect(() => {
  const id = view.value.session?.id ?? null;
  if (sessionId !== undefined && id !== sessionId) {
    selection.value = [];
    highlighted.value = [];
    zoomRequest.value = null;
  }
  sessionId = id;
});

/** For the tests */
export function resetSelection() {
  selection.value = [];
  highlighted.value = [];
  zoomRequest.value = null;
}
