// The views of the design in tabs of the workspace (docs/plan_design_views.md): V3 the floorplan,
// V4 the pins, V5 the routing
import { floorplanView } from "../views/floorplan";
import { pinsView } from "../views/pins";
import { routingView } from "../views/routing";
import { ViewPanel } from "./ViewPanel";

export const FloorplanPanel = () => (
  <ViewPanel name="floorplan" title="Placement" def={floorplanView} />
);
export const PinsPanel = () => (
  <ViewPanel name="pins" title="Floorplan" def={pinsView} />
);
export const RoutingPanel = () => (
  <ViewPanel name="routing" title="Routing" def={routingView} />
);
