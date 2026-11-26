import { useEffect, useState } from "preact/hooks";
import type { DesignView, DevicePackageInfo, Hierarchy, HierarchyModule, HierarchyNode, HierarchySource } from "../api";
import {
  addDesign,
  busy,
  config,
  devicePackages,
  hierarchy,
  loadHierarchy,
  loadPackageIntoSession,
  makeSessionFromPackage,
  openFile,
  openPanel,
  view,
} from "../store";

/** The words of a search: every one must be in the text of an entry (any case) */
export function matches(query: string, ...texts: (string | null | undefined)[]): boolean {
  const haystack = texts.filter(Boolean).join(" ").toLowerCase();
  return query
    .toLowerCase()
    .split(/\s+/)
    .filter(Boolean)
    .every((word) => haystack.includes(word));
}

function Search(props: { label: string; value: string; onInput: (text: string) => void; shown: number; total: number }) {
  return (
    <div class="panel-search">
      <input
        type="search"
        aria-label={props.label}
        placeholder={props.label}
        value={props.value}
        onInput={(e) => props.onInput((e.target as HTMLInputElement).value)}
      />
      <span class="muted panel-count">
        {props.shown === props.total ? props.total : `${props.shown} of ${props.total}`}
      </span>
    </div>
  );
}

const packageText = (p: DevicePackageInfo) => [p.name, p.description, p.layout, p.lut_size != null ? `K${p.lut_size}` : "", ...p.tags];

/** The device packages that were found, with a search: a session is created from one, or an empty
 * session reads its architectures */
export function DevicesPanel() {
  const [query, setQuery] = useState("");
  const found = devicePackages.value;
  const real = config.value?.backend === "session";
  const v = view.value;
  const empty = !!v.session && v.session.worker === "running" && v.files.inputs.length === 0;
  const current = v.session?.device_package?.name;
  const shown = found ? found.packages.filter((p) => matches(query, ...packageText(p))) : [];
  return (
    <section class="panel" aria-label="Device packages">
      <Search label="Search the device packages" value={query} onInput={setQuery} shown={shown.length} total={found?.packages.length ?? 0} />
      {found === null ? (
        <div class="muted">Looking…</div>
      ) : (
        <div class="panel-list">
          {found.packages.length === 0 && <div class="muted">No device packages. Start the app with --device-packages FOLDER.</div>}
          {found.packages.length > 0 && shown.length === 0 && <div class="muted">No device package matches “{query}”.</div>}
          {shown.map((p) => (
            <div key={p.name} class={`panel-item ${p.name === current ? "current" : ""}`}>
              <div class="panel-item-head">
                <strong>{p.name}</strong>
                <span class="chip">{p.layout}</span>
                {p.lut_size != null && <span class="chip">K{p.lut_size}</span>}
                {p.version && <span class="muted">v{p.version}</span>}
                {p.name === current && <span class="chip chip-current">this session</span>}
                <span class="spacer" />
                <button disabled={!real || busy.value} onClick={() => void makeSessionFromPackage(p.name)} title={real ? "A new session that reads its architectures" : "Not in the demo"}>
                  Create session
                </button>
                {empty && (
                  <button disabled={busy.value} onClick={() => void loadPackageIntoSession(p.name)} title="Read its architectures into the open session">
                    Load into this session
                  </button>
                )}
              </div>
              {p.description && <div class="muted panel-item-text">{p.description}</div>}
              {p.tags.length > 0 && <div class="muted panel-item-text">{p.tags.join(" · ")}</div>}
            </div>
          ))}
          {found.problems
            .filter((p) => matches(query, p.name, p.message))
            .map((p) => (
              <div key={p.name} class="panel-item package-bad" title={p.message}>
                <strong>{p.name}</strong>{" "}
                <span class="muted">cannot be used: {p.message.replace(/^The device package '[^']*' cannot be used: /, "")}</span>
              </div>
            ))}
        </div>
      )}
    </section>
  );
}

/** The folder of a design directory: the last part of its path */
const folderOf = (d: DesignView) => d.path.split("/").filter(Boolean).pop() ?? d.name;

/** The design directories of the designs folders, with a search. Add copies one into the session;
 * it is not synthesized (the run button of synthesis does that) */
export function DesignsPanel() {
  const [query, setQuery] = useState("");
  const v = view.value;
  const designs = v.designs ?? [];
  const real = config.value?.backend === "session";
  const session = v.session;
  const copied = new Set((v.files.design ?? []).map((f) => f.name.split("/")[1]).filter(Boolean));
  const shown = designs.filter((d) => matches(query, d.name, d.top, d.path, d.description, ...d.files, ...(d.netlists ?? [])));
  return (
    <section class="panel" aria-label="Design directories">
      <Search label="Search the designs" value={query} onInput={setQuery} shown={shown.length} total={designs.length} />
      <div class="panel-list">
        {designs.length === 0 && <div class="muted">No design directory: a folder with a design.json in a designs folder (--designs-dir).</div>}
        {designs.length > 0 && shown.length === 0 && <div class="muted">No design matches “{query}”.</div>}
        {shown.map((d) => {
          const added = copied.has(folderOf(d));
          return (
            <div key={d.path} class={`panel-item ${added ? "current" : ""}`}>
              <div class="panel-item-head">
                <strong>{d.name}</strong>
                {d.top && <span class="chip">top {d.top}</span>}
                <span class="chip">
                  {d.files.length ? `${d.files.length} ${d.files.length === 1 ? "RTL file" : "RTL files"}` : d.netlists?.length ? "netlist only" : "no RTL"}
                </span>
                {added && <span class="chip chip-current">in this session</span>}
                <span class="spacer" />
                <button
                  disabled={!real || busy.value || !session}
                  title={
                    !real
                      ? "Not in the demo"
                      : !session
                        ? "Open a session first"
                        : `Copy this design into the session${added ? " again (in place of its copy)" : ""}; it is not synthesized`
                  }
                  onClick={() => {
                    if (added && !window.confirm(`The session has a copy of ${d.name}. Copy it again, in place of that copy?`)) return;
                    void addDesign(d.name);
                  }}
                >
                  {added ? "Add again" : "Add to session"}
                </button>
              </div>
              {d.description && <div class="muted panel-item-text">{d.description}</div>}
              <div class="muted panel-item-text panel-path">{d.path}</div>
            </div>
          );
        })}
      </div>
    </section>
  );
}

/** The parameters of a module, as the RTL writes them: W=8, N=2 */
const parametersText = (parameters?: Record<string, number | string>) =>
  Object.entries(parameters ?? {})
    .map(([k, v]) => `${k}=${v}`)
    .join(", ");

/** The line of the RTL of a module or an instance, opened in the viewer */
function SourceLink(props: { src?: HierarchySource | null }) {
  const src = props.src;
  if (!src?.file) return null;
  const file = src.file;
  return (
    <button
      class="link hier-src"
      title={`Open ${file} at line ${src.line}`}
      onClick={() => void openFile(file, src.line)}
    >
      {`${file.split("/").pop()}:${src.line}`}
    </button>
  );
}

const KIND_TITLES = {
  library: "A cell of the device package (its cell library): a blackbox",
  unknown: "No file of the design has this module",
};

function KindChip(props: { kind: HierarchyNode["kind"] }) {
  if (props.kind === "module") return null;
  return (
    <span class={`chip hier-${props.kind}`} title={KIND_TITLES[props.kind]}>
      {props.kind}
    </span>
  );
}

/** The tree with only the instances that match the search, and their parents */
export function filterTree(
  node: HierarchyNode,
  query: string,
): HierarchyNode | null {
  if (!query.trim()) return node;
  const children = node.children
    .map((c) => filterTree(c, query))
    .filter((c): c is HierarchyNode => c !== null);
  if (
    children.length ||
    matches(query, node.instance, node.module, parametersText(node.parameters))
  ) {
    return {
      ...node,
      children,
      more: children.length === node.children.length ? node.more : undefined,
    };
  }
  return null;
}

const countNodes = (node: HierarchyNode | null): number =>
  node ? 1 + node.children.reduce((sum, c) => sum + countNodes(c), 0) : 0;

interface RowProps {
  node: HierarchyNode;
  path: string;
  depth: number;
  open: Set<string>;
  toggle: (path: string) => void;
  all: boolean;
  compact: boolean;
}

function TreeRow(props: RowProps) {
  const { node, path, depth, compact } = props;
  const has = node.children.length > 0 || !!node.more;
  const expanded = props.all || props.open.has(path);
  const params = parametersText(node.parameters);
  const name = node.instance ?? node.module;
  const src = node.src?.file ? node.src : null;
  return (
    <li
      class="hier-node"
      role="treeitem"
      aria-expanded={has ? expanded : undefined}
      aria-level={depth + 1}
    >
      <div
        class="hier-row"
        title={
          compact
            ? `${name}: ${node.module}${params ? ` #(${params})` : ""}`
            : undefined
        }
      >
        {has ? (
          <button
            class="hier-toggle"
            aria-label={`${expanded ? "Collapse" : "Expand"} ${name}`}
            onClick={() => props.toggle(path)}
            disabled={props.all}
          >
            {expanded ? "▾" : "▸"}
          </button>
        ) : (
          <span class="hier-toggle" aria-hidden="true" />
        )}
        {compact && src ? (
          // In the sidebar the name opens its line: there is no room for file:line
          <button
            class="link hier-instance"
            title={`Open ${src.file} at line ${src.line}`}
            onClick={() => void openFile(src.file as string, src.line)}
          >
            {name}
          </button>
        ) : (
          <strong class="hier-instance">{name}</strong>
        )}
        {node.instance && <span class="hier-module muted">{node.module}</span>}
        {params && !compact && (
          <span class="hier-params muted">#({params})</span>
        )}
        <KindChip kind={node.kind} />
        {node.children.length > 0 && !compact && (
          <span class="muted hier-count">
            {node.children.length + (node.more ?? 0)}
          </span>
        )}
        {!compact && (
          <>
            <span class="spacer" />
            <SourceLink src={node.src} />
          </>
        )}
      </div>
      {has && expanded && (
        <ul role="group" class="hier-children">
          {node.children.map((c, i) => (
            <TreeRow
              key={`${path}/${i}`}
              {...props}
              node={c}
              path={`${path}/${i}`}
              depth={depth + 1}
            />
          ))}
          {!!node.more && (
            <li
              class="hier-node muted hier-more"
              title="The tree is cut at its limit, breadth first"
            >
              … {node.more} more {node.more === 1 ? "instance" : "instances"}
            </li>
          )}
        </ul>
      )}
    </li>
  );
}

/** The tree of the instances of a design. The first two levels are open (the first one in the
 * sidebar); a search shows the instances that match, with their parents, all open */
export function HierarchyTree(props: {
  data: Hierarchy;
  query?: string;
  compact?: boolean;
}) {
  // The nodes that the person opened, for the data that they were opened in: another elaboration
  // is back to the levels that are open at first
  const [open, setOpen] = useState<{
    of: Hierarchy;
    paths: Set<string>;
  } | null>(null);
  const compact = !!props.compact;
  const query = props.query ?? "";
  const opened =
    open && open.of === props.data
      ? open.paths
      : new Set(
          compact
            ? ["0"]
            : ["0", ...props.data.tree.children.map((_, i) => `0/${i}`)],
        );
  const toggle = (path: string) => {
    const next = new Set(opened);
    if (next.has(path)) next.delete(path);
    else next.add(path);
    setOpen({ of: props.data, paths: next });
  };
  const tree = filterTree(props.data.tree, query);
  if (!tree) return <div class="muted">No instance matches “{query}”.</div>;
  return (
    <ul
      class={`hier-tree ${compact ? "hier-compact" : ""}`}
      role="tree"
      aria-label={`The instances of ${props.data.top}`}
    >
      <TreeRow
        node={tree}
        path="0"
        depth={0}
        open={opened}
        toggle={toggle}
        all={!!query.trim()}
        compact={compact}
      />
    </ul>
  );
}

/** The hierarchy of the design that the session added last, elaborated again when the session or
 * its design changes (not while a call runs: the worker does one thing at a time). Gives the state,
 * null when it is of another session or design */
export function useHierarchy() {
  const state = hierarchy.value;
  const real = config.value?.backend === "session";
  const v = view.value;
  const session = v.session?.id ?? null;
  const design = v.synthesis?.design ?? null;
  const stale = state.session !== session || state.design !== design;
  useEffect(() => {
    if (real && session && design && stale && !state.loading && !busy.value)
      void loadHierarchy();
  }, [real, session, design, stale, state.loading, busy.value]);
  return {
    real,
    session,
    design,
    state,
    data: stale ? null : state.data,
    error: stale ? null : state.error,
  };
}

function ModulesTable(props: { modules: HierarchyModule[]; query: string }) {
  const shown = props.modules.filter((m) =>
    matches(props.query, m.module, m.kind, parametersText(m.parameters)),
  );
  return (
    <div class="hier-table">
      <table class="hier-modules">
        <thead>
          <tr>
            <th>Module</th>
            <th>Parameters</th>
            <th class="num">Instances</th>
            <th
              class="num"
              title="The cells of Yosys in one instance: its logic, not its instances"
            >
              Cells
            </th>
            <th>Source</th>
          </tr>
        </thead>
        <tbody>
          {shown.map((m) => (
            <tr key={m.name}>
              <td>
                {m.module} <KindChip kind={m.kind} />
              </td>
              <td class="muted">{parametersText(m.parameters)}</td>
              <td class="num">{m.instances}</td>
              <td class="num">{m.kind === "module" ? m.cells : ""}</td>
              <td>
                <SourceLink src={m.src} />
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

const counts = (data: Hierarchy) =>
  `${data.instances} ${data.instances === 1 ? "instance" : "instances"}, ${data.modules.length} ${data.modules.length === 1 ? "module" : "modules"}`;

/** What is said instead of the tree: not in the demo, no session or design, an error, loading */
function HierarchyStatus(props: { h: ReturnType<typeof useHierarchy> }) {
  const { real, session, design, state, error } = props.h;
  if (!real) return <div class="muted">Not in the demo.</div>;
  if (!session)
    return (
      <div class="muted">Open a session, and add a design to it (Designs).</div>
    );
  if (!design)
    return (
      <div class="muted">
        Add a design to the session (Designs) to see its modules and instances.
      </div>
    );
  if (error && !state.loading)
    return (
      <div class="hier-error" role="alert">
        {error.message}
        {error.hint && <div class="muted">{error.hint}</div>}
      </div>
    );
  return <div class="muted">Elaborating the design with Yosys…</div>;
}

/** The hierarchy in the sidebar: the tree, small, with buttons to elaborate it again and to open
 * it in a tab (with the search and the modules) */
export function HierarchySection() {
  const h = useHierarchy();
  const { data, state } = h;
  return (
    <div class="hier-side">
      {data && (
        <div class="hier-side-head">
          <span class="muted">{counts(data)}</span>
          {data.truncated && <span class="muted"> ({data.nodes} shown)</span>}
          <span class="spacer" />
          <button
            class="stage-button"
            aria-label="Elaborate the hierarchy again"
            title="Elaborate the RTL again (a separate run of Yosys: the flow is not changed)"
            disabled={state.loading || busy.value}
            onClick={() => void loadHierarchy()}
          >
            ↻
          </button>
          <button
            class="stage-button"
            aria-label="Open the hierarchy in a tab"
            title="Open in a tab: search, modules, lines"
            onClick={() => openPanel("hierarchy")}
          >
            ⤢
          </button>
        </div>
      )}
      {data ? <HierarchyTree data={data} compact /> : <HierarchyStatus h={h} />}
    </div>
  );
}

/** The hierarchy of the design that the session added last: the tree of its instances, or its
 * modules with how many instances each has. Yosys elaborates the RTL in a run of its own, and the
 * flow is not changed */
export function HierarchyPanel() {
  const [query, setQuery] = useState("");
  const [mode, setMode] = useState<"tree" | "modules">("tree");
  const h = useHierarchy();
  const { data, state, real } = h;
  const modules = data
    ? data.modules.filter((m) =>
        matches(query, m.module, m.kind, parametersText(m.parameters)),
      )
    : [];
  return (
    <section class="panel hier" aria-label="Design hierarchy">
      <div class="panel-item-head hier-head">
        {data ? (
          <>
            <strong>{data.design ?? data.top}</strong>
            <span class="chip">top {data.top}</span>
            <span class="muted">{counts(data)}</span>
            {data.truncated && (
              <span
                class="chip"
                title="The tree shows the first instances, breadth first"
              >
                {data.nodes} shown
              </span>
            )}
          </>
        ) : (
          <strong>Hierarchy</strong>
        )}
        <span class="spacer" />
        {data && (
          <div class="hier-mode" role="group" aria-label="View">
            <button
              aria-pressed={mode === "tree"}
              onClick={() => setMode("tree")}
            >
              Tree
            </button>
            <button
              aria-pressed={mode === "modules"}
              onClick={() => setMode("modules")}
            >
              Modules
            </button>
          </div>
        )}
        <button
          disabled={!real || !h.design || state.loading || busy.value}
          title={
            real
              ? "Elaborate the RTL again (a separate run of Yosys: the flow is not changed)"
              : "Not in the demo"
          }
          onClick={() => void loadHierarchy()}
        >
          {state.loading ? "Elaborating…" : "Refresh"}
        </button>
      </div>
      {!data ? (
        <HierarchyStatus h={h} />
      ) : (
        <>
          <Search
            label={
              mode === "tree" ? "Search the instances" : "Search the modules"
            }
            value={query}
            onInput={setQuery}
            shown={
              mode === "tree"
                ? countNodes(filterTree(data.tree, query))
                : modules.length
            }
            total={mode === "tree" ? data.nodes : data.modules.length}
          />
          {mode === "tree" ? (
            <HierarchyTree data={data} query={query} />
          ) : (
            <ModulesTable modules={data.modules} query={query} />
          )}
        </>
      )}
    </section>
  );
}
