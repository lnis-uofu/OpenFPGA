/************************************************************************
 * A header file for SimulationSetting class, including critical data declaration
 * Please include this file only for using any SimulationSetting data structure
 * Refer to simulation_setting.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for Simulation Clock to avoid illegal type casting 
 ***********************************************************************/
#ifndef SIMULATION_SETTING_FWD_H
#define SIMULATION_SETTING_FWD_H

#include "vtr_strong_id.h"

struct simulation_clock_id_tag;

typedef vtr::StrongId<simulation_clock_id_tag> SimulationClockId;

/* Short declaration of class */
class SimulationSetting;

#endif
