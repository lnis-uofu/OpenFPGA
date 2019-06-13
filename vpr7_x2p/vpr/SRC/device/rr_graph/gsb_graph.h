/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    gsb_graph.h
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/12  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file constains a class to model the connection of a 
 *  General Switch Block (GSB), which is a unified block of Connection Blocks 
 *  and Switch Blocks.
 *  This block contains
 *  1. A switch block
 *  2. A X-direction Connection block locates at the left side of the switch block
 *  2. A Y-direction Connection block locates at the top side of the switch block   
 *  
 *                                +---------------------------------+
 *                                |          Y-direction CB         |
 *                                |              [x][y + 1]         |
 *                                +---------------------------------+
 *                   
 *                                           TOP SIDE
 *   +-------------+              +---------------------------------+
 *   |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODES           OPIN_NODES |
 *   | X-direction |              |                                 |
 *   |      CB     |  LEFT SIDE   |        Switch Block             |  RIGHT SIDE
 *   |   [x][y]    |              |              [x][y]             |
 *   |             |              |                                 |
 *   |             |              | CHAN_NODES           CHAN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODES           OPIN_NODES |
 *   |             |              |                                 |
 *   |             |              | OPIN_NODE CHAN_NODES OPIN_NODES |
 *   +-------------+              +---------------------------------+
 *                                              BOTTOM SIDE
 *
 ***********************************************************************/

/* IMPORTANT:
 * The following preprocessing flags are added to 
 * avoid compilation error when this headers are included in more than 1 times 
 */
#ifndef GSB_GRAPH_H
#define GSB_GRAPH_H

/*
 * Notes in include header files in a head file 
 * Only include the neccessary header files 
 * that is required by the data types in the function/class declarations!
 */
/* Header files should be included in a sequence */
/* Standard header files required go first */
#include <vector>

/* External library header files */
#include "vtr_vector.h"
#include "vtr_range.h"

#include "device_coordinator.h"
#include "vpr_types.h"
#include "rr_graph_fwd.h"

/* Define open nodes */
#define OPEN_NODE_ID RRNodeId(-1)
#define OPEN_EDGE_ID RREdgeId(-1)

/***********************************************************************
 *  This data structure focuses on modeling the internal pin-to-pin connections.
 *  It is basically a collection of nodes and edges.
 *  To make the data structure general, the nodes and edges are not linked to any another data 
 *  structures.
 *
 *  node_ids_: a collection of nodes (basically ids) modelling routing tracks 
 *             which locate at each side of the GSB <0..num_nodes_per_side-1>
 *
 *  node_directions_: Indicate if this node is an input or an output of the GSB 
 *                  <0..num_nodes_per_side-1>
 *
 *  node_types_: specify the types of the node, CHANX|CHANY|IPIN|OPIN  
 *
 *  node_sides_: specify the sides of the node on a GSB, TOP|RIGHT|BOTTOM|LEFT 
 *
 *  node_grid_sides_: specify the side of the node on which side of a GRID  
 *                  for CHANX and CHANY, it is an invalid value 
 *                  <0..num_nodes_per_side-1>
 *
 *  node_in_edges_: indcies of input edges of a node 
 *                  <0..num_nodes><0..num_input_edgess-1>
 *
 *  node_out_edges_: indcies of output edges of a node
 *                   <0..num_nodes><0..num_output_edges-1>
 *
 *  edge_ids_: a collection of indices of edges, <0..num_edges-1>, which connects the nodes
 *  
 *  edge_src_nodes_: indcies of input nodes of an edge (driving nodes for each edge) 
 *            <0..num_input_nodes-1>
 *
 *  edge_sink_nodes_: indices of output nodes of an edge (fan-out nodes for each edge) 
 *            <0..num_output_nodes-1>
 *
 ***********************************************************************/

class GSBGraph {
  public: /* Types */
    typedef vtr::vector<RRNodeId, RRNodeId>::const_iterator node_iterator;
    typedef vtr::vector<RREdgeId, RREdgeId>::const_iterator edge_iterator;
    typedef vtr::Range<node_iterator> node_range;
    typedef vtr::Range<edge_iterator> edge_range;
  public: /* Constructors */
    GSBGraph(const GSBGraph&);  /* A constructor to duplicate */ 
    GSBGraph();
  public: /* Accessors */
    /* Aggregates */
    node_range nodes() const;
    edge_range edges() const;
  private: /* Internal Data */
    /* Coordinator of this GSB */
    DeviceCoordinator coordinator_;

    /* nodes on each side  */
    vtr::vector<RRNodeId, RRNodeId> node_ids_;
    vtr::vector<RRNodeId, t_rr_type> node_types_;
    vtr::vector<RRNodeId, enum e_side> node_sides_;
    vtr::vector<RRNodeId, enum e_direction> node_directions_; 
    vtr::vector<RRNodeId, enum e_side> node_grid_sides_;
   
    vtr::vector<RRNodeId, std::vector<RREdgeId>> node_in_edges;
    vtr::vector<RRNodeId, std::vector<RREdgeId>> node_out_edges;
 
    /* edges  */
    vtr::vector<RREdgeId, RREdgeId> edge_ids_;
    vtr::vector<RREdgeId, RREdgeId> edge_src_nodes_; /* each element is a node_id */
    vtr::vector<RREdgeId, RREdgeId> edge_sink_nodes_; /* each element is a node_id */
};

#endif

