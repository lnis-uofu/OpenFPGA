import { fireEvent, render, screen, within } from "@testing-library/preact";
import { beforeEach, describe, expect, it } from "vitest";
import { ClusterList } from "./components/ClusterList";
import { ContextMenu, contextMenu } from "./components/ContextMenu";
import { resetStore } from "./store";
import { resetViewData, setWholeView } from "./views/data";
import { forgetLayouts } from "./views/layout";
import { type NetlistBase, clustersView } from "./views/netlist";
import { atoms, clusters } from "./views/netlist.fixtures";
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
  setWholeView("clusters", clusters());
  base = await clustersView.load();
  changes = 0;
});

const show = () =>
  render(
    <div>
      <ClusterList base={base} changed={() => changes++} version={0} />
      <ContextMenu />
    </div>,
  );
const row = (name: string) =>
  screen
    .getByText(name, { selector: ".name" })
    .closest(".cluster-row") as HTMLElement;

describe("the list of the clusters (V2)", () => {
  it("lists the clusters, and opens one into its atoms on a double click", () => {
    show();
    expect(screen.getByText("4 clusters")).toBeTruthy();
    expect(within(row("clb_a")).getByText("clb · 3")).toBeTruthy();
    expect(screen.queryByText("u_a.x1")).toBeNull();
    fireEvent.dblClick(row("clb_a"));
    expect(screen.getByText("u_a.x1")).toBeTruthy();
    expect(screen.getByText("$abc$7$x3")).toBeTruthy();
    fireEvent.dblClick(row("clb_a"));
    expect(screen.queryByText("u_a.x1")).toBeNull();
    // The arrow opens it too
    fireEvent.click(screen.getByLabelText("Open clb_q"));
    expect(screen.getByText("u_c.q")).toBeTruthy();
  });

  it("selects a cluster or an atom on a click, for the drawing to show it", () => {
    show();
    fireEvent.click(row("clb_a"));
    expect(selection.value).toEqual([{ kind: "block", name: "clb_a" }]);
    fireEvent.dblClick(row("clb_a"));
    fireEvent.click(row("u_a.u_b.x2"));
    expect(selection.value).toEqual([{ kind: "atom", name: "u_a.u_b.x2" }]);
    expect(row("u_a.u_b.x2").getAttribute("aria-selected")).toBe("true");
    expect(row("clb_a").getAttribute("aria-selected")).toBe("false");
  });

  it("opens the cluster of an atom selected elsewhere", async () => {
    show();
    selection.value = [{ kind: "atom", name: "u_c.q" }];
    expect(await screen.findByText("u_c.q")).toBeTruthy();
    expect(row("u_c.q").getAttribute("aria-selected")).toBe("true");
  });

  it("expands and collapses a cluster in the layout from a right click", () => {
    show();
    fireEvent.contextMenu(row("clb_a"), { clientX: 10, clientY: 20 });
    const menu = screen.getByRole("menu");
    expect(
      within(menu)
        .getAllByRole("menuitem")
        .map((m) => m.textContent),
    ).toEqual(["Expand in the layout", "Open in the list", "Zoom to it"]);
    fireEvent.click(within(menu).getByText("Expand in the layout"));
    expect(base.expanded.has("clb_a")).toBe(true);
    expect(changes).toBe(1);
    expect(screen.queryByRole("menu")).toBeNull();
    fireEvent.contextMenu(row("clb_a"));
    fireEvent.click(screen.getByText("Collapse in the layout"));
    expect(base.expanded.has("clb_a")).toBe(false);
  });

  it("filters by the name of a cluster, its type or an atom", () => {
    show();
    fireEvent.input(screen.getByLabelText("Filter the clusters"), {
      target: { value: "x3" },
    });
    expect(screen.getByText("1 of 4 clusters")).toBeTruthy();
    expect(row("clb_a")).toBeTruthy();
    fireEvent.input(screen.getByLabelText("Filter the clusters"), {
      target: { value: "io" },
    });
    expect(screen.getByText("2 of 4 clusters")).toBeTruthy();
  });
});
