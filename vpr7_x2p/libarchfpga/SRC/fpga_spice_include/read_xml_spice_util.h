#ifndef READ_XML_SPICE_UTIL_H 
#define READ_XML_SPICE_UTIL_H 

/* Declaration of Subroutines */
#include "my_free_fwd.h"

void InitSpiceMeasParams(t_spice_meas_params* meas_params);
void FreeSpiceMeasParams(t_spice_meas_params* meas_params);
void InitSpiceStimulateParams(t_spice_stimulate_params* stimulate_params);
void FreeSpiceStimulateParams(t_spice_stimulate_params* stimulate_params);
void InitSpiceParams(t_spice_params* spice_params);
void FreeSpiceParams(t_spice_params* params);
void FreeSpiceModelNetlist(t_spice_model_netlist* spice_model_netlist);
void FreeSpiceModelBuffer(t_spice_model_buffer* spice_model_buffer);
void FreeSpiceModelPassGateLogic(t_spice_model_pass_gate_logic* spice_model_pass_gate_logic);
void FreeSpiceModelPort(t_spice_model_port* spice_model_port);
void FreeSpiceModelWireParam(t_spice_model_wire_param* spice_model_wire_param);
void FreeSpiceModel(t_spice_model* spice_model);
void InitSpice(t_spice* spice);
void FreeSpice(t_spice* spice);
void FreeSpiceMuxArch(t_spice_mux_arch* spice_mux_arch);
void FreeSramInf(t_sram_inf* sram_inf);

#endif
