#pragma once
#include "SetupAnalysis.hpp"
#include "HoldAnalysis.hpp"
#include "tatum/delay_calc/DelayCalculator.hpp"
#include "tatum/graph_visitors/GraphVisitor.hpp"

namespace tatum {

/** \class SetupHoldAnalysis
 *
 * The SetupHoldAnalysis class defines the operations needed by a timing analyzer
 * to perform a combinded setup (max/long path) and hold (min/shortest path) analysis.
 *
 * Performing both analysis simultaneously tends to be more efficient than performing
 * them sperately due to cache locality.
 *
 * \see SetupAnalysis
 * \see HoldAnalysis
 * \see TimingAnalyzer
 */
class SetupHoldAnalysis : public GraphVisitor {
    public:
        SetupHoldAnalysis(size_t num_tags, size_t num_slacks)
            : setup_visitor_(num_tags, num_slacks)
            , hold_visitor_(num_tags, num_slacks) {}

        void do_reset_node(const NodeId node_id) override { 
            setup_visitor_.do_reset_node(node_id); 
            hold_visitor_.do_reset_node(node_id); 
        }

#ifdef TATUM_CALCULATE_EDGE_SLACKS
        void do_reset_edge(const EdgeId edge_id) override { 
            setup_visitor_.do_reset_edge(edge_id); 
            hold_visitor_.do_reset_edge(edge_id); 
        }
#endif

        void do_reset_node_arrival_tags(const NodeId node_id) override { 
            setup_visitor_.do_reset_node_arrival_tags(node_id); 
            hold_visitor_.do_reset_node_arrival_tags(node_id); 
        }

        void do_reset_node_required_tags(const NodeId node_id) override { 
            setup_visitor_.do_reset_node_required_tags(node_id); 
            hold_visitor_.do_reset_node_required_tags(node_id); 
        }

        void do_reset_node_slack_tags(const NodeId node_id) override { 
            setup_visitor_.do_reset_node_slack_tags(node_id); 
            hold_visitor_.do_reset_node_slack_tags(node_id); 
        }

        void do_reset_node_arrival_tags_from_origin(const NodeId node_id, const NodeId origin) override { 
            setup_visitor_.do_reset_node_arrival_tags_from_origin(node_id, origin); 
            hold_visitor_.do_reset_node_arrival_tags_from_origin(node_id, origin); 
        }

        void do_reset_node_required_tags_from_origin(const NodeId node_id, const NodeId origin) override { 
            setup_visitor_.do_reset_node_required_tags_from_origin(node_id, origin); 
            hold_visitor_.do_reset_node_required_tags_from_origin(node_id, origin); 
        }

        bool do_arrival_pre_traverse_node(const TimingGraph& tg, const TimingConstraints& tc, const NodeId node_id) override { 
            bool setup_unconstrained = setup_visitor_.do_arrival_pre_traverse_node(tg, tc, node_id); 
            bool hold_unconstrained = hold_visitor_.do_arrival_pre_traverse_node(tg, tc, node_id); 

            return setup_unconstrained || hold_unconstrained;
        }

        bool do_required_pre_traverse_node(const TimingGraph& tg, const TimingConstraints& tc, const NodeId node_id) override { 
            bool setup_unconstrained = setup_visitor_.do_required_pre_traverse_node(tg, tc, node_id); 
            bool hold_unconstrained = hold_visitor_.do_required_pre_traverse_node(tg, tc, node_id); 

            return setup_unconstrained || hold_unconstrained;
        }

        bool do_arrival_traverse_node(const TimingGraph& tg, const TimingConstraints& tc, const DelayCalculator& dc, const NodeId node_id) override { 
            bool setup_modified = setup_visitor_.do_arrival_traverse_node(tg, tc, dc, node_id); 
            bool hold_modified = hold_visitor_.do_arrival_traverse_node(tg, tc, dc, node_id); 

            return setup_modified || hold_modified;
        }

        bool do_required_traverse_node(const TimingGraph& tg, const TimingConstraints& tc, const DelayCalculator& dc, const NodeId node_id) override { 
            bool setup_modified = setup_visitor_.do_required_traverse_node(tg, tc, dc, node_id); 
            bool hold_modified = hold_visitor_.do_required_traverse_node(tg, tc, dc, node_id); 

            return setup_modified || hold_modified;
        }
        
        bool do_slack_traverse_node(const TimingGraph& tg, const DelayCalculator& dc, const NodeId node) override {
            bool setup_modified = setup_visitor_.do_slack_traverse_node(tg, dc, node); 
            bool hold_modified = hold_visitor_.do_slack_traverse_node(tg, dc, node); 

            return setup_modified || hold_modified;
        }

        TimingTags::tag_range setup_tags(const NodeId node_id) const { return setup_visitor_.setup_tags(node_id); }
        TimingTags::tag_range setup_tags(const NodeId node_id, TagType type) const { return setup_visitor_.setup_tags(node_id, type); }
#ifdef TATUM_CALCULATE_EDGE_SLACKS
        TimingTags::tag_range setup_edge_slacks(const EdgeId edge_id) const { return setup_visitor_.setup_edge_slacks(edge_id); }
#endif
        TimingTags::tag_range setup_node_slacks(const NodeId node_id) const { return setup_visitor_.setup_node_slacks(node_id); }

        TimingTags::tag_range hold_tags(const NodeId node_id) const { return hold_visitor_.hold_tags(node_id); }
        TimingTags::tag_range hold_tags(const NodeId node_id, TagType type) const { return hold_visitor_.hold_tags(node_id, type); }
#ifdef TATUM_CALCULATE_EDGE_SLACKS
        TimingTags::tag_range hold_edge_slacks(const EdgeId edge_id) const { return hold_visitor_.hold_edge_slacks(edge_id); }
#endif
        TimingTags::tag_range hold_node_slacks(const NodeId node_id) const { return hold_visitor_.hold_node_slacks(node_id); }

        SetupAnalysis& setup_visitor() { return setup_visitor_; }
        HoldAnalysis& hold_visitor() { return hold_visitor_; }
    private:
        SetupAnalysis setup_visitor_;
        HoldAnalysis hold_visitor_;
};

} //namepsace
