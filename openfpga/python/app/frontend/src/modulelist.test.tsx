import { fireEvent, render, screen, within } from "@testing-library/preact";
import { beforeEach, describe, expect, it } from "vitest";
import { ContextMenu, contextMenu } from "./components/ContextMenu";
import { ModuleList } from "./components/ModuleList";
import { resetStore } from "./store";
import { resetViewData, setWholeView } from "./views/data";
import { forgetLayouts } from "./views/layout";
import { type NetlistBase, moduleTree, netlistView } from "./views/netlist";
import { atoms } from "./views/netlist.fixtures";
import { resetSelection, selection } from "./views/selection";

let base: NetlistBase;
let changes = 0;

beforeEach(async () => {
  resetStore();
  resetSelection();
  resetViewData();
  forgetLayouts();
  contextMenu.value = null;
  setWholeView("atoms", atoms());
  base = await netlistView.load();
  changes = 0;
});

const show = () =>
  render(
    <div>
      <ModuleList base={base} changed={() => changes++} version={0} />
      <ContextMenu />
    </div>,
  );
const row = (name: string) =>
  screen
    .getByText(name, { selector: ".name" })
    .closest(".cluster-row") as HTMLElement;

describe("the hierarchy of the modules (V1)", () => {
  it("makes the tree of the modules from the module of each atom", () => {
    const tree = moduleTree(base);
    expect(tree.count).toBe(6);
    expect(tree.children.map((m) => [m.key, m.count])).toEqual([
      ["u_a", 3],
      ["u_c", 1],
    ]);
    const a = tree.children[0];
    expect(a.children.map((m) => [m.key, m.label, m.count])).toEqual([
      ["u_a.u_b", "u_b", 2],
    ]);
    expect(moduleTree(base)).toBe(tree); // made once
  });

  it("lists the modules, and opens one into its modules and atoms on a double click", () => {
    show();
    expect(screen.getByText("3 modules, 6 atoms")).toBeTruthy();
    expect(row("u_a")).toBeTruthy();
    expect(screen.queryByText("u_b")).toBeNull();
    fireEvent.dblClick(row("u_a"));
    expect(row("u_b")).toBeTruthy();
    fireEvent.dblClick(row("u_b"));
    expect(screen.getByText("u_a.u_b.x2")).toBeTruthy();
    fireEvent.dblClick(row("u_b"));
    expect(screen.queryByText("u_a.u_b.x2")).toBeNull();
    // The arrow opens it too
    fireEvent.click(screen.getByLabelText("Open u_c"));
    expect(screen.getByText("u_c.q")).toBeTruthy();
  });

  it("selects a module or an atom on a click, for the drawing to show it", () => {
    show();
    fireEvent.click(row("u_a"));
    expect(selection.value).toEqual([{ kind: "module", name: "u_a" }]);
    fireEvent.dblClick(row("u_a"));
    fireEvent.click(row("u_a.x1"));
    expect(selection.value).toEqual([{ kind: "atom", name: "u_a.x1" }]);
    expect(row("u_a.x1").getAttribute("aria-selected")).toBe("true");
    expect(row("u_a").getAttribute("aria-selected")).toBe("false");
  });

  it("opens the modules of an atom selected elsewhere", async () => {
    show();
    selection.value = [{ kind: "atom", name: "u_a.u_b.x2" }];
    expect(await screen.findByText("u_a.u_b.x2")).toBeTruthy();
    expect(row("u_a.u_b.x2").getAttribute("aria-selected")).toBe("true");
  });

  it("expands and collapses a module in the layout from a right click on the list", () => {
    show();
    fireEvent.contextMenu(row("u_a"), { clientX: 10, clientY: 20 });
    const menu = screen.getByRole("menu");
    expect(
      within(menu)
        .getAllByRole("menuitem")
        .map((m) => m.textContent),
    ).toEqual(["Expand in the layout", "Open in the list", "Zoom to it"]);
    fireEvent.click(within(menu).getByText("Expand in the layout"));
    expect(base.expanded.has("u_a")).toBe(true);
    expect(changes).toBe(1);
    fireEvent.contextMenu(row("u_a"));
    fireEvent.click(screen.getByText("Collapse in the layout"));
    expect(base.expanded.has("u_a")).toBe(false);
  });

  it("filters by a module, an atom or a model, and opens what matches", () => {
    show();
    fireEvent.input(screen.getByLabelText("Filter the modules"), {
      target: { value: "u_a.u_b.x2" },
    });
    expect(row("u_a.u_b.x2")).toBeTruthy();
    expect(screen.queryByText("u_c")).toBeNull();
    fireEvent.input(screen.getByLabelText("Filter the modules"), {
      target: { value: "zzz" },
    });
    expect(screen.getByText(/no match/)).toBeTruthy();
  });
});
