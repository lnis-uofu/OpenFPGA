/********************************************************************
 * This file includes functions that are used to annotate pb_graph_node
 * and pb_graph_pins from VPR to OpenFPGA
 *******************************************************************/
#include <cmath>
#include <iterator>

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "timing_info.h"
#include "AnalysisDelayCalculator.h"
#include "net_delay.h"

#include "annotate_simulation_setting.h"

/* begin namespace openfpga */
namespace openfpga {

constexpr int MIN_NUM_SIM_OPERATING_CLOCK_CYCLES = 2;

/********************************************************************
 * Find the average signal density for all the nets of user's benchmark
 *******************************************************************/
static 
float average_atom_net_signal_density(const AtomContext& atom_ctx,
                                      const std::unordered_map<AtomNetId, t_net_power>& net_activity) {
  float avg_density = 0.;
  size_t net_cnt = 0;

  /* get the average density of all the nets */
  for (const AtomNetId& atom_net : atom_ctx.nlist.nets()) {
    /* Skip the nets without any activity annotation */
    if (0 == net_activity.count(atom_net)) {
      continue;
    }

    /* Only care non-zero density nets */
    if (0. == net_activity.at(atom_net).density) {
      continue;
    }

    avg_density += net_activity.at(atom_net).density; 
    net_cnt++;
  }

  return avg_density / net_cnt; 
}                                     

/********************************************************************
 * Find the average signal density for all the nets of user's benchmark
 * by applying a weight to each net density
 *******************************************************************/
static 
float average_weighted_atom_net_signal_density(const AtomContext& atom_ctx,
                                               const std::unordered_map<AtomNetId, t_net_power>& net_activity) {

  float weighted_avg_density = 0.;
  size_t weighted_net_cnt = 0;

  /* get the average density of all the nets */
  for (const AtomNetId& atom_net : atom_ctx.nlist.nets()) {
    /* Skip the nets without any activity annotation */
    if (0 == net_activity.count(atom_net)) {
      continue;
    }

    /* Only care non-zero density nets */
    if (0. == net_activity.at(atom_net).density) {
      continue;
    }

    /* Consider the weight of fan-out */
    size_t net_weight; 
    if (0 == std::distance(atom_ctx.nlist.net_sinks(atom_net).begin(), atom_ctx.nlist.net_sinks(atom_net).end())) {
      net_weight = 1;
    } else {
      VTR_ASSERT(0 < std::distance(atom_ctx.nlist.net_sinks(atom_net).begin(), atom_ctx.nlist.net_sinks(atom_net).end()));
      net_weight = std::distance(atom_ctx.nlist.net_sinks(atom_net).begin(), atom_ctx.nlist.net_sinks(atom_net).end());
    }
    weighted_avg_density += net_activity.at(atom_net).density* net_weight;
    weighted_net_cnt += net_weight;
  }

  return weighted_avg_density / weighted_net_cnt; 
}

/********************************************************************
 * Find median of signal density of all the nets 
 *******************************************************************/
static 
size_t median_atom_net_signal_density(const AtomContext& atom_ctx,
                                      const std::unordered_map<AtomNetId, t_net_power>& net_activity) {
  /* Sort the net density */
  std::vector<float> net_densities;

  net_densities.reserve(net_activity.size());

  for (const AtomNetId& atom_net : atom_ctx.nlist.nets()) {
    /* Skip the nets without any activity annotation */
    if (0 == net_activity.count(atom_net)) {
      continue;
    }

    net_densities.push_back(net_activity.at(atom_net).density);
  }
  std::sort(net_densities.begin(), net_densities.end());

  /* Get the median */
  /* check for even case */
  if (net_densities.size() % 2 != 0) { 
    return net_densities[size_t(net_densities.size() / 2)];
  }            
  
  return 0.5 * (net_densities[size_t((net_densities.size() - 1) / 2)] + net_densities[size_t((net_densities.size() - 1) / 2)]);
}

/********************************************************************
 * Find the number of clock cycles in simulation based on the average signal density
 *******************************************************************/
static 
size_t recommend_num_sim_clock_cycle(const AtomContext& atom_ctx,
                                     const std::unordered_map<AtomNetId, t_net_power>& net_activity, 
                                     const float& sim_window_size) {

  float average_density = average_atom_net_signal_density(atom_ctx, net_activity);
  float average_weighted_density = average_weighted_atom_net_signal_density(atom_ctx, net_activity);
  float median_density = median_atom_net_signal_density(atom_ctx, net_activity);

  VTR_LOG("Average net density: %.2f\n",
          average_density);
  VTR_LOG("Median net density: %.2f\n",
          median_density);
  VTR_LOG("Average net density after weighting: %.2f\n",
          average_weighted_density);

  /* We have three choices in selecting the number of clock cycles based on signal density
   * 1. average signal density 
   * 2. median signal density
   * 3. a mixed of average and median signal density 
   */
  size_t recmd_num_clock_cycles = 0;
  if ( (0. == median_density) 
    && (0. == average_density) ) {
    recmd_num_clock_cycles = 1;
    VTR_LOG_WARN("All the signal density is zero!\nNumber of clock cycles in simulations are set to be %ld!\n",
                 recmd_num_clock_cycles);
  } else if (0. == average_density) {
    recmd_num_clock_cycles = (size_t)round(1 / median_density); 
  } else if (0. == median_density) {
    recmd_num_clock_cycles = (size_t)round(1 / average_density);
  } else {
    /* add a sim window size to balance the weight of average density and median density
     * In practice, we find that there could be huge difference between avereage and median values 
     * For a reasonable number of simulation clock cycles, we do this window size.
     */
    recmd_num_clock_cycles = (size_t)round(1 / (sim_window_size * average_density + (1 - sim_window_size) * median_density ));

    VTR_LOG("Window size set for simulation: %.2f\n",
            sim_window_size);
    VTR_LOG("Net density after applying window size : %.2f\n", 
            (sim_window_size * average_density + (1 - sim_window_size) * median_density));
  }
  
  VTR_ASSERT(0 < recmd_num_clock_cycles);

  /* Minimum number of clock cycles should be at least 2
   * so that self-testing testbench can work!!!
   */
  if (MIN_NUM_SIM_OPERATING_CLOCK_CYCLES > recmd_num_clock_cycles) {
    recmd_num_clock_cycles = MIN_NUM_SIM_OPERATING_CLOCK_CYCLES;
  }

  return recmd_num_clock_cycles; 
}

/********************************************************************
 * Annotate simulation setting based on VPR results
 *  - If the operating clock frequency is set to follow the vpr timing results,
 *    we will set a new operating clock frequency here
 *  - If the number of clock cycles in simulation is set to be automatically determined,
 *    we will infer the number based on the average signal density
 *******************************************************************/
int annotate_simulation_setting(const AtomContext& atom_ctx, 
                                const std::unordered_map<AtomNetId, t_net_power>& net_activity, 
                                SimulationSetting& sim_setting) {

  /* Find if the operating frequency is binded to vpr results */
  if (0. == sim_setting.operating_clock_frequency()) {
    VTR_LOG("User specified the operating clock frequency to use VPR results\n");
    /* Run timing analysis and collect critical path delay
     * This code is copied from function vpr_analysis() in vpr_api.h 
     * Should keep updated to latest VPR code base
     * Note:
     *   - MUST mention in documentation that VPR should be run in timing enabled mode
     */
    vtr::vector<ClusterNetId, float*> net_delay;
    vtr::t_chunk net_delay_ch;
    /* Load the net delays */
    net_delay = alloc_net_delay(&net_delay_ch);
    load_net_delay_from_routing(net_delay);

    /* Do final timing analysis */
    auto analysis_delay_calc = std::make_shared<AnalysisDelayCalculator>(atom_ctx.nlist, atom_ctx.lookup, net_delay);
    auto timing_info = make_setup_hold_timing_info(analysis_delay_calc);
    timing_info->update();

    /* Get critical path delay. Update simulation settings */
    float T_crit = timing_info->least_slack_critical_path().delay() * (1. + sim_setting.operating_clock_frequency_slack());
    sim_setting.set_operating_clock_frequency(1 / T_crit); 
    VTR_LOG("Use VPR critical path delay %g [ns] with a %g [%] slack in OpenFPGA.\n",
            T_crit / 1e9, sim_setting.operating_clock_frequency_slack() * 100);
  }
  VTR_LOG("Will apply operating clock frequency %g [MHz] to simulations\n",
          sim_setting.operating_clock_frequency() / 1e6);

  if (0. == sim_setting.num_clock_cycles()) {
    /* Find the number of clock cycles to be used in simulation 
     * by average over the signal activity 
     */
    VTR_LOG("User specified the number of operating clock cycles to be inferred from signal activities\n");

    /* Use a fixed simulation window size now. TODO: this could be specified by users */
    size_t num_clock_cycles = recommend_num_sim_clock_cycle(atom_ctx,
                                                            net_activity, 
                                                            0.5);
    sim_setting.set_num_clock_cycles(num_clock_cycles);
  }

  /* Minimum number of clock cycles should be at least 2
   * so that self-testing testbench can work!!!
   */
  if (MIN_NUM_SIM_OPERATING_CLOCK_CYCLES > sim_setting.num_clock_cycles()) {
    VTR_LOG_ERROR("Minimum number of operating clock cycles applicable to simulations is %d, while the specified number is %lu!\n",
                  MIN_NUM_SIM_OPERATING_CLOCK_CYCLES,
                  sim_setting.num_clock_cycles());
    return CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOG("Will apply %lu operating clock cycles to simulations\n",
          sim_setting.num_clock_cycles());

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
