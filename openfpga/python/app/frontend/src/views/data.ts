// The whole exports that the views share (device, placement, pins, clusters), fetched once for a
// state of the session and kept until it changes (docs/plan_design_views.md, 3.1)
import { type ViewData, fetchView } from "./format";
import { forgetLayouts } from "./layout";

let signature = "";
const cache = new Map<string, Promise<ViewData>>();
const ready = new Map<string, ViewData>();

/** The state of the session that the data is of: when it changes, everything is fetched again */
export function setDataSignature(next: string) {
  if (next === signature) return;
  signature = next;
  cache.clear();
  ready.clear();
  // A layout is kept by its graph: the graph of another design may have the same nodes
  forgetLayouts();
}

type Scope = Record<string, string | number>;

const keyOf = (kind: string, scope: Scope) =>
  Object.keys(scope).length
    ? `${kind}?${new URLSearchParams(Object.entries(scope).map(([k, v]) => [k, String(v)]))}`
    : kind;

/** A whole export (of a scope: the number of paths of the timing...), fetched once for the state
 * of the session */
export function wholeView(kind: string, scope: Scope = {}): Promise<ViewData> {
  const key = keyOf(kind, scope);
  let found = cache.get(key);
  if (!found) {
    const asked = signature;
    found = fetchView(kind, scope).then((data) => {
      if (asked === signature) ready.set(key, data);
      return data;
    });
    found.catch(() => cache.delete(key));
    cache.set(key, found);
  }
  return found;
}

/** A whole export if it is there already; else it is asked for, and then() runs when it comes */
export function wholeViewNow(
  kind: string,
  then: () => void,
  scope: Scope = {},
): ViewData | undefined {
  const found = ready.get(keyOf(kind, scope));
  if (found) return found;
  wholeView(kind, scope).then(then, () => undefined);
  return undefined;
}

/** For the tests: a whole export, as if it was fetched */
export function setWholeView(kind: string, data: ViewData, scope: Scope = {}) {
  const key = keyOf(kind, scope);
  cache.set(key, Promise.resolve(data));
  ready.set(key, data);
}

/** For the tests */
export function resetViewData() {
  signature = "";
  cache.clear();
  ready.clear();
}
