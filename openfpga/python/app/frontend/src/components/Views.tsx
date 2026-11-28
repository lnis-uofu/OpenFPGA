// The views of the design in tabs of the workspace (docs/plan_design_views.md): V3 the floorplan,
// V4 the pins, V5 the routing, V1 the netlist, V2 the clusters
import { floorplanView } from "../views/floorplan";
import { clustersView, netlistView } from "../views/netlist";
import { pinsView } from "../views/pins";
import { routingView } from "../views/routing";
import { ClusterList } from "./ClusterList";
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
export const NetlistPanel = () => (
  <ViewPanel name="netlist" title="Netlist" def={netlistView} />
);
/** V2 with the list of its clusters beside the drawing */
const clustersWithList = {
  ...clustersView,
  side: { label: "List", component: ClusterList },
};
export const ClustersPanel = () => (
  <ViewPanel name="clusters" title="Clusters" def={clustersWithList} />
);
