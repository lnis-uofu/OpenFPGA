/***********************************************
 * This file includes functions to generate
 * Verilog submodules for multiplexers.
 * including both fundamental submodules
 * such as a branch in a multiplexer 
 * and the full multiplexer
 **********************************************/

#include "util.h"
#include "vtr_assert.h"

#include "verilog_submodule_mux.h"


/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
static 
void generate_verilog_cmos_mux_branch_module_structural(std::fstream& fp,
                                                        const CircuitLibrary& circuit_lib, 
                                                        const CircuitModelId& circuit_model, 
                                                        const MuxGraph& mux_graph) {
  return;
}

/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
void generate_verilog_mux_branch_module(std::fstream& fp, 
                                        const CircuitLibrary& circuit_lib, 
                                        const CircuitModelId& circuit_model, 
                                        const MuxGraph& mux_graph) {
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(circuit_model)) {
  case SPICE_MODEL_DESIGN_CMOS:
    if (true == circuit_lib.dump_structural_verilog(circuit_model)) {
      generate_verilog_cmos_mux_branch_module_structural(fp, circuit_lib, circuit_model, mux_graph);
    } else {
      /*
      dump_verilog_cmos_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             mux_size,
                                             num_input_basis_subckt,
                                             cur_spice_model,
                                             special_basis);
       */
    }
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* If requested, we can dump structural verilog for basis module */
    /*
    if (true == circuit_lib.dump_structural_verilog(circuit_model)) {
      dump_verilog_rram_mux_one_basis_module_structural(fp, mux_basis_subckt_name,
                                                        num_input_basis_subckt,
                                                        cur_spice_model);
    } else {
      dump_verilog_rram_mux_one_basis_module(fp, mux_basis_subckt_name,
                                             num_input_basis_subckt,
                                             cur_spice_model);
    }
    */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.circuit_model_name(circuit_model)); 
    exit(1);
  }

  return;
}
