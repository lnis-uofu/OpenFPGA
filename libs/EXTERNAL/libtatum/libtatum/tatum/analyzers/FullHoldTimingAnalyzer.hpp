#pragma once
#include "tatum/graph_walkers/SerialWalker.hpp"
#include "tatum/HoldAnalysis.hpp"
#include "tatum/analyzers/HoldTimingAnalyzer.hpp"
#include "tatum/base/validate_timing_graph_constraints.hpp"

namespace tatum { namespace detail {

/**
 * A concrete implementation of a HoldTimingAnalyzer.
 *
 * This is a full (i.e. non-incremental) analyzer, which fully
 * re-analyzes the timing graph whenever update_timing_impl() is 
 * called.
 */
template<class GraphWalker=SerialWalker>
class FullHoldTimingAnalyzer : public HoldTimingAnalyzer {
    public:
        FullHoldTimingAnalyzer(const TimingGraph& timing_graph, const TimingConstraints& timing_constraints, const DelayCalculator& delay_calculator)
            : HoldTimingAnalyzer()
            , timing_graph_(timing_graph)
            , timing_constraints_(timing_constraints)
            , delay_calculator_(delay_calculator)
            , hold_visitor_(timing_graph_.nodes().size(), timing_graph_.edges().size()) {
            validate_timing_graph_constraints(timing_graph_, timing_constraints_);

            //Initialize profiling data
            graph_walker_.set_profiling_data("total_analysis_sec", 0.);
            graph_walker_.set_profiling_data("analysis_sec", 0.);
            graph_walker_.set_profiling_data("num_full_updates", 0.);
        }

    protected:
        virtual void update_timing_impl() override {
            update_hold_timing();
        }

        virtual void update_hold_timing_impl() override {
            auto start_time = Clock::now();

            graph_walker_.do_reset(timing_graph_, hold_visitor_);

            graph_walker_.do_arrival_pre_traversal(timing_graph_, timing_constraints_, hold_visitor_);            
            graph_walker_.do_arrival_traversal(timing_graph_, timing_constraints_, delay_calculator_, hold_visitor_);            

            graph_walker_.do_required_pre_traversal(timing_graph_, timing_constraints_, hold_visitor_);            
            graph_walker_.do_required_traversal(timing_graph_, timing_constraints_, delay_calculator_, hold_visitor_);            

            graph_walker_.do_update_slack(timing_graph_, delay_calculator_, hold_visitor_);

            double analysis_sec = std::chrono::duration_cast<dsec>(Clock::now() - start_time).count();

            //Record profiling data
            double total_analysis_sec = analysis_sec + graph_walker_.get_profiling_data("total_analysis_sec");
            graph_walker_.set_profiling_data("total_analysis_sec", total_analysis_sec);
            graph_walker_.set_profiling_data("analysis_sec", analysis_sec);
            graph_walker_.set_profiling_data("num_full_updates", graph_walker_.get_profiling_data("num_full_updates") + 1);
        }

        virtual void invalidate_edge_impl(const EdgeId edge) override {
            graph_walker_.invalidate_edge(edge);
        }

        virtual node_range modified_nodes_impl() const override {
            return graph_walker_.modified_nodes();
        }

        double get_profiling_data_impl(std::string key) const override { return graph_walker_.get_profiling_data(key); }
        size_t num_unconstrained_startpoints_impl() const override { return graph_walker_.num_unconstrained_startpoints(); }
        size_t num_unconstrained_endpoints_impl() const override { return graph_walker_.num_unconstrained_endpoints(); }

        TimingTags::tag_range hold_tags_impl(NodeId node_id) const override { return hold_visitor_.hold_tags(node_id); }
        TimingTags::tag_range hold_tags_impl(NodeId node_id, TagType type) const override { return hold_visitor_.hold_tags(node_id, type); }
#ifdef TATUM_CALCULATE_EDGE_SLACKS
        TimingTags::tag_range hold_edge_slacks_impl(EdgeId edge_id) const override { return hold_visitor_.hold_edge_slacks(edge_id); }
#endif
        TimingTags::tag_range hold_node_slacks_impl(NodeId node_id) const override { return hold_visitor_.hold_node_slacks(node_id); }

    private:
        const TimingGraph& timing_graph_;
        const TimingConstraints& timing_constraints_;
        const DelayCalculator& delay_calculator_;
        HoldAnalysis hold_visitor_;
        GraphWalker graph_walker_;

        typedef std::chrono::duration<double> dsec;
        typedef std::chrono::high_resolution_clock Clock;
};

}} //namepsace
