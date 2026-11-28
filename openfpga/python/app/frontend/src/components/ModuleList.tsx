// The list of the modules beside the drawing of V1, as the list of the clusters is beside V2: the
// hierarchy of the design, each module opening in the list into its modules and its atoms (its
// arrow, or a double click), a click selects a module or an atom (and the drawing shows it), and
// a right click expands or collapses the module in the layout. The list follows the selection:
// the modules of an atom selected elsewhere open.
import { useEffect, useMemo, useRef, useState } from "preact/hooks";
import {
  type ModuleNode,
  type NetlistBase,
  groupOpen,
  moduleTree,
  setGroupOpen,
} from "../views/netlist";
import { moduleKeys } from "../views/netgraph";
import { type ObjectRef, refKey } from "../views/scene";
import { runViewCommand, selection } from "../views/selection";
import { type MenuItem, openMenu } from "./ContextMenu";

/** The rows drawn at most under one module: a filter narrows a longer list */
export const MODULE_LIST_LIMIT = 400;

export function ModuleList(props: {
  base: NetlistBase;
  changed: () => void;
  version: number;
}) {
  const { base, changed } = props;
  const tree = useMemo(() => moduleTree(base), [base, base.netlist]);
  const [open, setOpen] = useState<Set<string>>(() => new Set([""]));
  const [filter, setFilter] = useState("");
  const list = useRef<HTMLUListElement>(null);
  const n = base.netlist;
  const chosen = new Set(selection.value.map(refKey));

  // The selection: the modules of a selected atom or module open in the list, and the first
  // selected row is scrolled to
  useEffect(() => {
    const more: string[] = [];
    for (const ref of selection.value) {
      let path: string | undefined;
      if (ref.kind === "atom") {
        const a = n.atomIndex.get(ref.name);
        path = a === undefined ? undefined : n.module[a];
      } else if (ref.kind === "module") path = ref.name;
      if (path === undefined) continue;
      // An atom opens its module; a module opens the ones around it
      const keys =
        ref.kind === "atom" ? moduleKeys(path) : moduleKeys(path).slice(0, -1);
      for (const key of ["", ...keys]) if (!open.has(key)) more.push(key);
    }
    if (more.length) setOpen(new Set([...open, ...more]));
    requestAnimationFrame(() =>
      list.current
        ?.querySelector<HTMLElement>('[aria-selected="true"]')
        ?.scrollIntoView?.({ block: "nearest" }),
    );
  }, [selection.value]);

  const toggle = (key: string) => {
    const next = new Set(open);
    if (next.has(key)) next.delete(key);
    else next.add(key);
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
  const moduleMenu = (m: ModuleNode): MenuItem[] => [
    groupOpen(base, m.key)
      ? {
          label: "Collapse in the layout",
          title: `The module ${m.key} as one box`,
          run: () => setGroupOpen(base, m.key, false, changed),
        }
      : {
          label: "Expand in the layout",
          title: `The atoms and modules of ${m.key}`,
          run: () => setGroupOpen(base, m.key, true, changed),
        },
    {
      label: open.has(m.key) ? "Close in the list" : "Open in the list",
      title: "What is in it, in this list",
      run: () => toggle(m.key),
    },
    zoom({ kind: "module", name: m.key }),
  ];

  // The filter: a module is shown when its name, an atom in it or a module in it matches, and a
  // module that has a match in it is open
  const text = filter.trim().toLowerCase();
  const matches = useMemo(() => {
    if (!text) return null;
    const found = new Map<string, boolean>();
    const visit = (m: ModuleNode): boolean => {
      let hit = m.key.toLowerCase().includes(text);
      for (const a of m.atoms)
        if (
          n.atoms[a].toLowerCase().includes(text) ||
          n.model[a].toLowerCase().includes(text)
        )
          hit = true;
      for (const c of m.children) if (visit(c)) hit = true;
      found.set(m.key, hit);
      return hit;
    };
    visit(tree);
    return found;
  }, [tree, text]);
  const shows = (m: ModuleNode) => !matches || matches.get(m.key) === true;
  const atomShown = (a: number) =>
    !matches ||
    n.atoms[a].toLowerCase().includes(text) ||
    n.model[a].toLowerCase().includes(text);

  const row = (m: ModuleNode, depth: number) => {
    const isTop = m.key === "";
    const isOpen = !!matches || open.has(m.key);
    const inLayout = !isTop && groupOpen(base, m.key);
    const ref: ObjectRef = { kind: "module", name: m.key };
    const pad = { paddingLeft: `${8 + depth * 14}px` };
    const kids = m.children.filter(shows);
    const atoms = m.atoms.filter(atomShown);
    return (
      <li key={m.key} role="treeitem" aria-expanded={isOpen}>
        <div
          class="cluster-row"
          style={pad}
          aria-selected={!isTop && chosen.has(refKey(ref))}
          title={
            isTop
              ? `The top: ${m.count} atoms`
              : `${m.key}: ${m.count} atoms${inLayout ? ", expanded in the layout" : ""}`
          }
          onClick={(e) =>
            isTop
              ? toggle("")
              : select(ref, e.shiftKey || e.ctrlKey || e.metaKey)
          }
          onDblClick={() => !isTop && toggle(m.key)}
          onContextMenu={(e) => !isTop && openMenu(e, moduleMenu(m))}
        >
          <button
            class="twisty"
            aria-label={`${isOpen ? "Close" : "Open"} ${isTop ? "the top" : m.key}`}
            onClick={(e) => {
              e.stopPropagation();
              toggle(m.key);
            }}
            onDblClick={(e) => e.stopPropagation()}
          >
            {isOpen ? "▾" : "▸"}
          </button>
          <span class="name">{m.label}</span>
          {inLayout && (
            <span class="open-mark" title="Expanded in the layout">
              ◇
            </span>
          )}
          <span class="muted">{m.count}</span>
        </div>
        {isOpen && (
          <ul role="group">
            {kids.slice(0, MODULE_LIST_LIMIT).map((c) => row(c, depth + 1))}
            {atoms.slice(0, MODULE_LIST_LIMIT).map((a) => {
              const atom: ObjectRef = { kind: "atom", name: n.atoms[a] };
              return (
                <li key={`a:${a}`} role="treeitem">
                  <div
                    class="cluster-row atom"
                    style={{ paddingLeft: `${26 + depth * 14}px` }}
                    aria-selected={chosen.has(refKey(atom))}
                    title={`${atom.name} (${n.model[a]})`}
                    onClick={(e) =>
                      select(atom, e.shiftKey || e.ctrlKey || e.metaKey)
                    }
                    onContextMenu={(e) => openMenu(e, [zoom(atom)])}
                  >
                    <span class="name">{atom.name}</span>
                    <span class="muted">{n.model[a]}</span>
                  </div>
                </li>
              );
            })}
            {(kids.length > MODULE_LIST_LIMIT ||
              atoms.length > MODULE_LIST_LIMIT) && (
              <li
                class="muted more"
                style={{ paddingLeft: `${26 + depth * 14}px` }}
              >
                …more: use the filter
              </li>
            )}
          </ul>
        )}
      </li>
    );
  };

  const modules = (m: ModuleNode): number =>
    m.children.reduce((sum, c) => sum + 1 + modules(c), 0);
  return (
    <>
      <input
        type="search"
        aria-label="Filter the modules"
        placeholder="Filter by module, atom or model"
        value={filter}
        onInput={(e) => setFilter((e.target as HTMLInputElement).value)}
      />
      <div class="side-count muted">
        {modules(tree)} modules, {tree.count} atoms
        {matches && !matches.get("") && " (no match)"}
      </div>
      <ul class="cluster-list" ref={list} role="tree" aria-label="Modules">
        {row(tree, 0)}
      </ul>
    </>
  );
}
