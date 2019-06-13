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
 * Filename:    rr_graph_gsb.c
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/06/12  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/
/************************************************************************
 *  This file constains the member functions for class GSBConn 
 ************************************************************************/

#include "gsb_graph.h"
#include "vtr_vector_map.h"

/************************************************************************
 *  Constructors for class GSBGraph
 ************************************************************************/
/* Duplicate a object */
GSBGraph::GSBGraph(const GSBGraph& gsb_graph) {

}

GSBGraph::GSBGraph() {
  coordinator_.clear();
  
}


/************************************************************************
 *  Aggregators for class GSBGraph 
 ************************************************************************/
/* Accessors: Aggregators */
GSBGraph::node_range GSBGraph::nodes() const {
  return vtr::make_range(node_ids_.begin(), node_ids_.end());
}

GSBGraph::edge_range GSBGraph::edges() const {
  return vtr::make_range(edge_ids_.begin(), edge_ids_.end());
}
