#ifndef READ_XML_SPICE_UTIL_H 
#define READ_XML_SPICE_UTIL_H 

/* Declaration of Subroutines */
#include "my_free_fwd.h"

void InitSpiceMeasParams(t_spice_meas_params* meas_params);
void FreeSpiceMeasParams();
void InitSpiceStimulateParams(t_spice_stimulate_params* stimulate_params);
void FreeSpiceStimulateParams();
void InitSpiceVariationParams(t_spice_mc_variation_params* mc_variation_params);
void FreeSpiceVariationParams();
void InitSpiceMonteCarloParams(t_spice_mc_params* mc_params);
void FreeSpiceMonteCarloParams();
void InitSpiceParams(t_spice_params* spice_params);
void FreeSpiceParams();
void FreeSpiceModelNetlist(t_spice_model_netlist* spice_model_netlist);
void FreeSpiceModelBuffer();
void FreeSpiceModelPassGateLogic();
void FreeSpiceModelPort(t_spice_model_port* spice_model_port);
void FreeSpiceModelWireParam();
void FreeSpiceModel(t_spice_model* spice_model);
void InitSpice(t_spice* spice);
void FreeSpice(t_spice* spice);
void FreeSpiceMuxArch(t_spice_mux_arch* spice_mux_arch);
void FreeSramInf(t_sram_inf* sram_inf);

#endif
