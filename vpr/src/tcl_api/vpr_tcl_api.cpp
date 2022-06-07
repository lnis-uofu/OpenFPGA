#include "vpr_tcl_api.h"

int vpr_tcl_api::parse_cmd()
{
    std::string cmd = cmd_v.back();
    char c;
    int i = 0;
    buffer_ = (char *)malloc(cmd.size() + 1);
    strcpy(buffer_, cmd.c_str());
    std::vector<char *> words;
    bool was_space = true;
    while (buffer_[i])
    {
        c = buffer_[i];
        if (isspace(c)) // We can have the character '=' as a separator here
        {
            buffer_[i] = '\0';
            was_space = true;
        }
        else
        {
            if (was_space)
            {
                words.push_back(&buffer_[i]);
            }
            was_space = false;
        }
        i++;
    }
    argc_ = words.size() + 1;
    argv_ = (char **)malloc(argc_ * sizeof(char **));
    argv_[0] = prog_name;
    for (unsigned int idx = 0; idx < words.size(); ++idx)
    {
        argv_[idx+1] = words[idx];
    }
    return 0;
}

int vpr_tcl_api::project_init(std::string cmd)
{
    cmd_v.push_back(cmd);
    if (parse_cmd())
        return ERROR_EXIT_CODE;
    try
    {
       vpr_install_signal_handler();
        /* Read options, architecture, and circuit netlist */
       vpr_init(argc_, (const char**)argv_, &Options, &vpr_setup, &Arch);
        if (Options.show_version)
        {
            return SUCCESS_EXIT_CODE;
        }
    }
    catch (const tatum::Error &tatum_error)
    {
        VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());

        return ERROR_EXIT_CODE;
    }
    catch (const VprError &vpr_error)
    {
        vpr_print_error(vpr_error);

        if (vpr_error.type() == VPR_ERROR_INTERRUPTED)
        {
            return INTERRUPTED_EXIT_CODE;
        }
        else
        {
            return ERROR_EXIT_CODE;
        }
    }
    catch (const vtr::VtrError &vtr_error)
    {
        VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(), vtr_error.what());

        return ERROR_EXIT_CODE;
    }

    /* Signal success to scripts */
    return SUCCESS_EXIT_CODE;
}

bool vpr_tcl_api::vpr_pack_flow(  )
{
    bool res = false;
    try
    {
        res = ::vpr_pack_flow(vpr_setup, Arch);
    }
    catch (const tatum::Error &tatum_error)
    {
        VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
        return false;
    }
    catch (const VprError &vpr_error)
    {
        vpr_print_error(vpr_error);
        return false;
    }
    catch (const vtr::VtrError &vtr_error)
    {
        VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(), vtr_error.what());
        return false;
    }
    return res;
}

bool vpr_tcl_api::vpr_place_flow(  )
{
    bool res = false;
    try
    {
        vpr_create_device(vpr_setup, Arch);
        res =  ::vpr_place_flow(vpr_setup, Arch);
    }
    catch (const tatum::Error &tatum_error)
    {
        VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
        return false;
    }
    catch (const VprError &vpr_error)
    {
        vpr_print_error(vpr_error);
        return false;
    }
    catch (const vtr::VtrError &vtr_error)
    {
        VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(), vtr_error.what());
        return false;
    }
    return res;
}

bool vpr_tcl_api::vpr_route_flow(  ){
    bool res = false;
    try
    {
        vpr_create_device(vpr_setup, Arch);
        route_status = ::vpr_route_flow(vpr_setup, Arch);
        res = route_status.success();
    }
    catch (const tatum::Error &tatum_error)
    {
        VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
        return false;
    }
    catch (const VprError &vpr_error)
    {
        vpr_print_error(vpr_error);
        return false;
    }
    catch (const vtr::VtrError &vtr_error)
    {
        VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(), vtr_error.what());
        return false;
    }
    return res;
}

bool vpr_tcl_api::vpr_analysis_flow(  )
{
    bool res = false;
    try
    {
        vpr_create_device(vpr_setup, Arch);
        res =  ::vpr_analysis_flow(vpr_setup, Arch, route_status);
    }
    catch (const tatum::Error &tatum_error)
    {
        VTR_LOG_ERROR("%s\n", format_tatum_error(tatum_error).c_str());
        return false;
    }
    catch (const VprError &vpr_error)
    {
        vpr_print_error(vpr_error);
        return false;
    }
    catch (const vtr::VtrError &vtr_error)
    {
        VTR_LOG_ERROR("%s:%d %s\n", vtr_error.filename_c_str(), vtr_error.line(), vtr_error.what());
        return false;
    }
    return res;
}

void vpr_tcl_api::vpr_timing_report(){
    auto& timing_ctx = ::g_vpr_ctx.timing();
    VTR_LOG("Timing analysis took %g seconds (%g STA, %g slack) (%zu full updates: %zu setup, %zu hold, %zu combined).\n",
            timing_ctx.stats.timing_analysis_wallclock_time(),
            timing_ctx.stats.sta_wallclock_time,
            timing_ctx.stats.slack_wallclock_time,
            timing_ctx.stats.num_full_updates(),
            timing_ctx.stats.num_full_setup_updates,
            timing_ctx.stats.num_full_hold_updates,
            timing_ctx.stats.num_full_setup_hold_updates);
}

vpr_tcl_api& api_env = vpr_tcl_api::getInstance();

int project_init(std::string cmd){
    return api_env.project_init(cmd);
}

bool pack_flow( ){
    return api_env.vpr_pack_flow();
}

bool place_flow( ){
    return api_env.vpr_place_flow();
}

bool route_flow( ){
    return api_env.vpr_route_flow();
}

bool analysis_flow(){
    return api_env.vpr_analysis_flow();
}

void timing_report(){
    api_env.vpr_timing_report();
}
