// The whole exports that the views share (device, placement, pins, clusters), fetched once for a
// state of the session and kept until it changes (docs/plan_design_views.md, 3.1)
import { type ViewData, fetchView } from "./format";

let signature = "";
const cache = new Map<string, Promise<ViewData>>();
const ready = new Map<string, ViewData>();

/** The state of the session that the data is of: when it changes, everything is fetched again */
export function setDataSignature(next: string) {
  if (next === signature) return;
  signature = next;
  cache.clear();
  ready.clear();
}

/** A whole export, fetched once for the state of the session */
export function wholeView(kind: string): Promise<ViewData> {
  let found = cache.get(kind);
  if (!found) {
    const asked = signature;
    found = fetchView(kind).then((data) => {
      if (asked === signature) ready.set(kind, data);
      return data;
    });
    found.catch(() => cache.delete(kind));
    cache.set(kind, found);
  }
  return found;
}

/** A whole export if it is there already; else it is asked for, and then() runs when it comes */
export function wholeViewNow(
  kind: string,
  then: () => void,
): ViewData | undefined {
  const found = ready.get(kind);
  if (found) return found;
  wholeView(kind).then(then, () => undefined);
  return undefined;
}

/** For the tests */
export function resetViewData() {
  signature = "";
  cache.clear();
  ready.clear();
}
