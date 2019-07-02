#ifndef SETUPVPR_H
#define SETUPVPR_H


void VPRSetupArch(t_arch* arch, 
                  t_det_routing_arch* RoutingArch,
		          t_segment_inf ** Segments,
                  t_swseg_pattern_inf** swseg_patterns,
                  t_model** user_models, 
                  t_model** library_models);

void alloc_and_init_globals_clb_to_clb_directs(int num_directs, 
                                               t_direct_inf* directs);

void SetupVPR(INP t_options *Options,
		INP boolean TimingEnabled,
		INP boolean readArchFile,
		OUTP struct s_file_name_opts *FileNameOpts,
		INOUTP t_arch * Arch,
		OUTP enum e_operation *Operation,
		OUTP t_model ** user_models,
		OUTP t_model ** library_models,
		OUTP struct s_packer_opts *PackerOpts,
		OUTP struct s_placer_opts *PlacerOpts,
		OUTP struct s_annealing_sched *AnnealSched,
		OUTP struct s_router_opts *RouterOpts,
		OUTP struct s_det_routing_arch *RoutingArch,
		OUTP t_segment_inf ** Segments,
		OUTP t_timing_inf * Timing,
		OUTP boolean * ShowGraphics,
		OUTP int *GraphPause,
		OUTP t_power_opts * power_opts,
        /*Xifan TANG: Switch Segment Pattern Support*/
        OUTP t_swseg_pattern_inf** swseg_patterns,
        /* Xifan TANG: FPGA-SPICE Support*/
        OUTP t_fpga_spice_opts* fpga_spice_opts);

void CheckSetup(INP enum e_operation Operation,
		INP struct s_placer_opts PlacerOpts,
		INP struct s_annealing_sched AnnealSched,
		INP struct s_router_opts RouterOpts,
		INP struct s_det_routing_arch RoutingArch,
		INP t_segment_inf * Segments,
		INP t_timing_inf Timing,
		INP t_chan_width_dist Chans);

void CheckArch(INP t_arch Arch,
		INP boolean TimingEnabled);

void CheckOptions(INP t_options Options,
		INP boolean TimingEnabled);

void ShowSetup(INP t_options options, INP t_vpr_setup vpr_setup);
void printClusteredNetlistStats(void);

#endif

