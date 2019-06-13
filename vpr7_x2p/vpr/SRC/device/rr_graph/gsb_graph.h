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

#include "device_coordinator.h"
#include "vpr_types.h"

/***********************************************************************
 *  This data structure focuses on modeling the internal pin-to-pin connections.
 *  It is basically a collection of nodes and edges.
 *  To make the data structure general, the nodes and edges are not linked to any another data 
 *  structures.
 *
 *  num_sides_: number of sides of this switch block
 *
 *  node_id_: a collection of nodes (basically ids) modelling routing tracks 
 *             which locate at each side of the GSB <0..num_sides-1><0..num_nodes_per_side-1>
 *
 *  node_direction_: Indicate if this node is an input or an output of the GSB 
 *                  <0..num_sides-1><0..num_nodes_per_side-1>
 *
 *  node_type_: specify the type of the node, CHANX|CHANY|IPIN|OPIN  
 *
 *  node_grid_side_: specify the side of the node on which side of a GRID  
 *                  for CHANX and CHANY, it is an invalid value 
 *                  <0..num_sides-1><0..num_nodes_per_side-1>
 *
 *  edge_id_: a collection of indices of edges, <0..num_edges-1>, which connects the nodes
 *  
 *  in_edge_: indcies of input nodes of an edge (driving nodes for each edge) 
 *            <0..num_edges-1><0..num_input_nodes-1>
 *
 *  out_edge_: indices of output nodes of an edge (fan-out nodes for each edge) 
 *            <0..num_edges-1><0..num_output_nodes-1>
 *
 ***********************************************************************/

class GSBGraph {
  public: /* Constructors */
    GSBGraph(const GSBGraph&);  /* A constructor to duplicate */ 
    GSBGraph();
  private: /* Internal Data */
    /* Coordinator of this GSB */
    DeviceCoordinator coordinator_;

    /* nodes on each side  */
    std::vector< std::vector<size_t> >  node_id_;
    std::vector< std::vector<t_rr_type> >  node_type_;
    std::vector< std::vector<enum PORTS> >  node_direction_; 
    std::vector< std::vector<enum e_side> > node_grid_side_;

    /* edges  */
    std::vector<size_t> edge_id_;
    std::vector< std::vector<size_t> > in_edge_; /* each element is a node_id */
    std::vector< std::vector<size_t> > out_edge_; /* each element is a node_id */
};

#endif

