#ifndef VPR_TCL_API_H
#define VPR_TCL_API_H
#include <cstdio>
#include <cstring>
#include <ctime>
#include <ctype.h>
#include <vector>
#include <string>
#include <string.h>

#include "vtr_error.h"
#include "vtr_memory.h"
#include "vtr_log.h"
#include "vtr_time.h"

#include "tatum/error.hpp"

#include "vpr_exit_codes.h"
#include "vpr_error.h"
#include "vpr_api.h"
#include "vpr_signal_handler.h"
#include "vpr_tatum_error.h"

#include "globals.h"

class vpr_tcl_api
{

    t_options Options;
    t_arch Arch;
    t_vpr_setup vpr_setup;
    char **argv_=nullptr;
    int argc_;
    char *buffer_=nullptr;
    RouteStatus route_status;
    char prog_name[19] = "vtr_tcl_interface";
    std::vector<std::string> cmd_v;
    vpr_tcl_api() { 
        vpr_install_signal_handler();
        Options = t_options();
        Arch = t_arch();
        vpr_setup = t_vpr_setup();
        }
    ~vpr_tcl_api() {
        if(buffer_) free(buffer_);
        if(argv_)   free(argv_);
    }
    vpr_tcl_api( const vpr_tcl_api& ) = delete;
    void operator=( const vpr_tcl_api& ) = delete;
public:
    static vpr_tcl_api& getInstance() {
        static vpr_tcl_api instance;
        return instance;  
    }
    int parse_cmd();
    int project_init(std::string cmd);
    /*
     * Stage operations
     */
    bool vpr_pack_flow( );    // Perform, load or skip the packing stage
/*
    bool vpr_pack( );         // Perform packing
    void vpr_load_packing( ); // Loads a previous packing
*/
    bool vpr_place_flow( );     // Perform, load or skip the placement stage
/*
    void vpr_place( );          // Perform placement
    void vpr_load_placement( ); // Loads a previous placement
*/
    bool vpr_route_flow( ); // Perform, load or skip the routing stage
/*
    RouteStatus vpr_route_fixed_W(t_vpr_setup &vpr_setup,
                                  const t_arch &arch,
                                  int fixed_channel_width,
                                  std::shared_ptr<SetupHoldTimingInfo> timing_info,
                                  std::shared_ptr<RoutingDelayCalculator> delay_calc,
                                  vtr::vector<ClusterNetId, float *> &net_delay); // Perform routing at a fixed channel width)
    RouteStatus vpr_route_min_W(t_vpr_setup &vpr_setup,
                                const t_arch &arch,
                                std::shared_ptr<SetupHoldTimingInfo> timing_info,
                                std::shared_ptr<RoutingDelayCalculator> delay_calc,
                                vtr::vector<ClusterNetId, float *> &net_delay); // Perform routing to find the minimum channel width
    RouteStatus vpr_load_routing(t_vpr_setup &vpr_setup,
                                 const t_arch &arch,
                                 int fixed_channel_width,
                                 std::shared_ptr<SetupHoldTimingInfo> timing_info,
                                 vtr::vector<ClusterNetId, float *> &net_delay); // Loads a previous routing
*/
    bool vpr_analysis_flow(); // Perform or skips the analysis stage
/*
    void vpr_analysis( , const RouteStatus &route_status);      // Perform post-implementation analysis
*/
    void vpr_timing_report();
};

int project_init(std::string cmd);
/*
* Stage operations
*/
bool pack_flow( );    // Perform, load or skip the packing stage

bool place_flow( );     // Perform, load or skip the placement stage

bool route_flow( ); // Perform, load or skip the routing stage

bool analysis_flow(); // Perform or skips the analysis stage

void timing_report();

#endif
