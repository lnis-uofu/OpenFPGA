// The list of the clusters beside the drawing of V2: each cluster opens in the list into its atoms
// (its arrow, or a double click), a click selects a cluster or an atom (and the drawing shows it),
// and a right click expands or collapses the cluster in the layout. The list follows the
// selection: a cluster selected elsewhere is scrolled to, the cluster of an atom opens.
import { useEffect, useMemo, useRef, useState } from "preact/hooks";
import {
  type ClusterContents,
  type NetlistBase,
  clusterContents,
  groupOpen,
  setGroupOpen,
} from "../views/netlist";
import { blockFill, css, viewColors } from "../views/palette";
import { type ObjectRef, refKey } from "../views/scene";
import { runViewCommand, selection } from "../views/selection";
import { type MenuItem, openMenu } from "./ContextMenu";

/** The rows drawn at most: a filter narrows a longer list */
export const LIST_LIMIT = 400;

export function ClusterList(props: {
  base: NetlistBase;
  changed: () => void;
  version: number;
}) {
  const { base, changed } = props;
  const clusters = useMemo(() => clusterContents(base), [base]);
  const [open, setOpen] = useState<Set<string>>(() => new Set());
  const [filter, setFilter] = useState("");
  const list = useRef<HTMLUListElement>(null);
  const n = base.netlist;
  const chosen = new Set(selection.value.map(refKey));

  // The selection: the cluster of a selected atom opens in the list, and the first selected row
  // is scrolled to
  useEffect(() => {
    const more: string[] = [];
    for (const ref of selection.value) {
      if (ref.kind !== "atom") continue;
      const a = n.atomIndex.get(ref.name);
      const name =
        a === undefined ? undefined : base.clusterName.get(n.cluster[a]);
      if (name && !open.has(name)) more.push(name);
    }
    if (more.length) setOpen(new Set([...open, ...more]));
    requestAnimationFrame(() =>
      list.current
        ?.querySelector<HTMLElement>('[aria-selected="true"]')
        ?.scrollIntoView?.({ block: "nearest" }),
    );
  }, [selection.value]);

  const toggle = (name: string) => {
    const next = new Set(open);
    if (next.has(name)) next.delete(name);
    else next.add(name);
    setOpen(next);
  };
  const select = (ref: ObjectRef, add: boolean) =>
    runViewCommand({ op: "select", kind: ref.kind, name: ref.name, add });
  const zoom = (ref: ObjectRef): MenuItem => ({
    label: "Zoom to it",
    title: `Show ${ref.name} in the views that have it`,
    run: () =>
      runViewCommand({ op: "zoom_to", kind: ref.kind, name: ref.name }),
  });
  const clusterMenu = (c: ClusterContents): MenuItem[] => [
    groupOpen(base, c.name)
      ? {
          label: "Collapse in the layout",
          title: `The cluster ${c.name} as one box`,
          run: () => setGroupOpen(base, c.name, false, changed),
        }
      : {
          label: "Expand in the layout",
          title: `The atoms of the cluster ${c.name}`,
          run: () => setGroupOpen(base, c.name, true, changed),
        },
    {
      label: open.has(c.name) ? "Close in the list" : "Open in the list",
      title: "Its atoms in this list",
      run: () => toggle(c.name),
    },
    zoom({ kind: "block", name: c.name }),
  ];

  const text = filter.trim().toLowerCase();
  const matching = text
    ? clusters.filter(
        (c) =>
          c.name.toLowerCase().includes(text) ||
          c.type.toLowerCase().includes(text) ||
          c.atoms.some((a) => n.atoms[a].toLowerCase().includes(text)),
      )
    : clusters;
  const shown = matching.slice(0, LIST_LIMIT);
  const dark = viewColors().dark;
  return (
    <>
      <input
        type="search"
        aria-label="Filter the clusters"
        placeholder="Filter by name, type or atom"
        value={filter}
        onInput={(e) => setFilter((e.target as HTMLInputElement).value)}
      />
      <div class="side-count muted">
        {matching.length === clusters.length
          ? `${clusters.length} clusters`
          : `${matching.length} of ${clusters.length} clusters`}
        {matching.length > shown.length && `, the first ${shown.length} shown`}
      </div>
      <ul class="cluster-list" ref={list} role="tree" aria-label="Clusters">
        {shown.map((c) => {
          const ref: ObjectRef = { kind: "block", name: c.name };
          const isOpen = open.has(c.name);
          const inLayout = groupOpen(base, c.name);
          return (
            <li key={c.name} role="treeitem" aria-expanded={isOpen}>
              <div
                class="cluster-row"
                aria-selected={chosen.has(refKey(ref))}
                title={`${c.name} (${c.type}): ${c.atoms.length} atoms${inLayout ? ", expanded in the layout" : ""}`}
                onClick={(e) =>
                  select(ref, e.shiftKey || e.ctrlKey || e.metaKey)
                }
                onDblClick={() => toggle(c.name)}
                onContextMenu={(e) => openMenu(e, clusterMenu(c))}
              >
                <button
                  class="twisty"
                  aria-label={`${isOpen ? "Close" : "Open"} ${c.name}`}
                  onClick={(e) => {
                    e.stopPropagation();
                    toggle(c.name);
                  }}
                  onDblClick={(e) => e.stopPropagation()}
                >
                  {isOpen ? "▾" : "▸"}
                </button>
                {c.typeIndex !== undefined && (
                  <span
                    class="swatch"
                    style={{ background: css(blockFill(c.typeIndex, dark)) }}
                  />
                )}
                <span class="name">{c.name}</span>
                {inLayout && (
                  <span class="open-mark" title="Expanded in the layout">
                    ◇
                  </span>
                )}
                <span class="muted">
                  {c.type} · {c.atoms.length}
                </span>
              </div>
              {isOpen && (
                <ul role="group">
                  {c.atoms.map((a) => {
                    const atom: ObjectRef = { kind: "atom", name: n.atoms[a] };
                    return (
                      <li key={a} role="treeitem">
                        <div
                          class="cluster-row atom"
                          aria-selected={chosen.has(refKey(atom))}
                          title={`${atom.name} (${n.model[a]})`}
                          onClick={(e) =>
                            select(atom, e.shiftKey || e.ctrlKey || e.metaKey)
                          }
                          onContextMenu={(e) =>
                            openMenu(e, [
                              ...(inLayout
                                ? []
                                : [
                                    {
                                      label: `Expand ${c.name} in the layout`,
                                      title: "Its atoms drawn one by one",
                                      run: () =>
                                        setGroupOpen(
                                          base,
                                          c.name,
                                          true,
                                          changed,
                                        ),
                                    },
                                  ]),
                              zoom(atom),
                            ])
                          }
                        >
                          <span class="name">{atom.name}</span>
                          <span class="muted">{n.model[a]}</span>
                        </div>
                      </li>
                    );
                  })}
                </ul>
              )}
            </li>
          );
        })}
      </ul>
    </>
  );
}
