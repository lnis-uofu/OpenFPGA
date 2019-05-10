#include <assert.h>
#include <string.h>
#include "util.h"
#include "vpr_types.h"
#include "OptionTokens.h"
#include "ReadOptions.h"
#include "globals.h"
#include "read_xml_arch_file.h"
#include "SetupVPR.h"
#include "pb_type_graph.h"
#include "ReadOptions.h"
/* mrFPGA: Xifan TANG*/
#include "mrfpga_api.h"
#include "mrfpga_globals.h"
/* END */

/* Xifan Tang: include for supporting Direct Parsing */
#include "vpr_utils.h"

static void SetupOperation(INP t_options Options,
		OUTP enum e_operation *Operation);
static void SetupPackerOpts(INP t_options Options, INP boolean TimingEnabled,
		INP t_arch Arch, INP char *net_file,
		OUTP struct s_packer_opts *PackerOpts);
static void SetupPlacerOpts(INP t_options Options, INP boolean TimingEnabled,
		OUTP struct s_placer_opts *PlacerOpts);
static void SetupAnnealSched(INP t_options Options,
		OUTP struct s_annealing_sched *AnnealSched);
static void SetupRouterOpts(INP t_options Options, INP boolean TimingEnabled,
		OUTP struct s_router_opts *RouterOpts);
static void SetupRoutingArch(INP t_arch Arch,
		OUTP struct s_det_routing_arch *RoutingArch);
static void SetupTiming(INP t_options Options, INP t_arch Arch,
		INP boolean TimingEnabled, INP enum e_operation Operation,
		INP struct s_placer_opts PlacerOpts,
		INP struct s_router_opts RouterOpts, OUTP t_timing_inf * Timing);
static void SetupSwitches(INP t_arch Arch,
		INOUTP struct s_det_routing_arch *RoutingArch,
		INP struct s_switch_inf *ArchSwitches, INP int NumArchSwitches);
static void SetupPowerOpts(t_options Options, t_power_opts *power_opts,
		t_arch * Arch);
/*Xifan TANG: SPICE Model Support*/
static void SetupSpiceOpts(t_options Options, 
                           t_spice_opts* spice_opts,
                           t_arch* arch);
/* end */
/* Xifan TANG: synthesizable verilog dumping */
static void SetupSynVerilogOpts(t_options Options, 
                                t_syn_verilog_opts* syn_verilog_opts,
                                t_arch* arch);

/* Xifan TANG: Bitstream Generator */
static void SetupBitstreamGenOpts(t_options Options, 
                                  t_bitstream_gen_opts* bitstream_gen_opts,
                                  t_arch* arch);

/* Xifan TANG: FPGA-SPICE Tool suites Options Setup */
static void SetupFpgaSpiceOpts(t_options Options, 
                               t_fpga_spice_opts* fpga_spice_opts,
                               t_arch* arch);
/* end */
/* Xifan Tang: Parse CLB to CLB direct connections */

/* mrFPGA */
static void SetupSwitches_mrFPGA(INP t_arch Arch,
		INOUTP struct s_det_routing_arch *RoutingArch,
		INP struct s_switch_inf *ArchSwitches, INP int NumArchSwitches, INP t_segment_inf* segment_inf);
static void setup_junction_switch(struct s_det_routing_arch *det_routing_arch);
static void add_wire_to_switch(struct s_det_routing_arch *det_routing_arch);
static void set_max_pins_per_side();
static void hack_switch_to_rram(struct s_det_routing_arch *det_routing_arch);
/* end */

void VPRSetupArch(t_arch* arch, 
                  t_det_routing_arch* RoutingArch,
		          t_segment_inf ** Segments,
                  /*Xifan TANG: Switch Segment Pattern Support*/
                  t_swseg_pattern_inf** swseg_patterns,
                  t_model** user_models, 
                  t_model** library_models) {
  int i, j;

  (*user_models) = arch->models;
  (*library_models) = arch->model_library;

  /* TODO: this is inelegant, I should be populating this information in XmlReadArch */
  EMPTY_TYPE = NULL;
  FILL_TYPE = NULL;
  IO_TYPE = NULL;
  for (i = 0; i < num_types; i++) {
    if (strcmp(type_descriptors[i].name, "<EMPTY>") == 0) {
      EMPTY_TYPE = &type_descriptors[i];
    } else if (strcmp(type_descriptors[i].name, "io") == 0) {
      IO_TYPE = &type_descriptors[i];
    } else {
      for (j = 0; j < type_descriptors[i].num_grid_loc_def; j++) {
        if (type_descriptors[i].grid_loc_def[j].grid_loc_type == FILL) {
          assert(FILL_TYPE == NULL);
          FILL_TYPE = &type_descriptors[i];
        }
      }
    }
  }
  assert(EMPTY_TYPE != NULL && FILL_TYPE != NULL && IO_TYPE != NULL);

  *Segments = arch->Segments;
  RoutingArch->num_segment = arch->num_segments;
  /*Xifan TANG: Switch Segment Pattern Support*/
  (*swseg_patterns) = arch->swseg_patterns;
  RoutingArch->num_swseg_pattern = arch->num_swseg_pattern;
  /* END */    

  /* mrFPGA */
  sync_arch_mrfpga_globals(arch->arch_mrfpga);
  if (is_mrFPGA) {
    SetupSwitches_mrFPGA(*arch, RoutingArch, 
                         arch->Switches, arch->num_switches, arch->Segments);
    /* Xifan TANG: added by bjxiao */
    set_max_pins_per_side();
    hack_switch_to_rram(RoutingArch);
  } else {
    /* Normal Setup VPR switches */
    // Xifan TANG: Add Connection Blocks Switches
    SetupSwitches(*arch, RoutingArch, 
                  arch->Switches, arch->num_switches);
  }
  /* END */    

  /* end */
  if(!is_mrFPGA && is_stack) {
    add_wire_to_switch(RoutingArch);
  }
  /* end */
  /* Xifan TANG: mrFPGA */
  if (is_junction) {
    setup_junction_switch(RoutingArch);
  }
  /* end */

  SetupRoutingArch(*arch, RoutingArch);

  return;
}

/* Sets VPR parameters and defaults. Does not do any error checking
 * as this should have been done by the various input checkers */
void SetupVPR(INP t_options *Options, INP boolean TimingEnabled,
		INP boolean readArchFile, OUTP struct s_file_name_opts *FileNameOpts,
		INOUTP t_arch * Arch, OUTP enum e_operation *Operation,
		OUTP t_model ** user_models, OUTP t_model ** library_models,
		OUTP struct s_packer_opts *PackerOpts,
		OUTP struct s_placer_opts *PlacerOpts,
		OUTP struct s_annealing_sched *AnnealSched,
		OUTP struct s_router_opts *RouterOpts,
		OUTP struct s_det_routing_arch *RoutingArch,
		OUTP t_segment_inf ** Segments, OUTP t_timing_inf * Timing,
		OUTP boolean * ShowGraphics, OUTP int *GraphPause,
		t_power_opts * PowerOpts,
        /*Xifan TANG: Switch Segment Pattern Support*/
        t_swseg_pattern_inf** swseg_patterns,
        t_fpga_spice_opts* fpga_spice_opts) {
	int len;

	len = strlen(Options->CircuitName) + 6; /* circuit_name.blif/0*/
	if (Options->out_file_prefix != NULL ) {
		len += strlen(Options->out_file_prefix);
	}
	default_output_name = (char*) my_calloc(len, sizeof(char));
	if (Options->out_file_prefix == NULL ) {
		sprintf(default_output_name, "%s", Options->CircuitName);
	} else {
		sprintf(default_output_name, "%s%s", Options->out_file_prefix,
				Options->CircuitName);
	}

	/* init default filenames */
	if (Options->BlifFile == NULL ) {
		len = strlen(Options->CircuitName) + 6; /* circuit_name.blif/0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->BlifFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->BlifFile, "%s.blif", Options->CircuitName);
		} else {
			sprintf(Options->BlifFile, "%s%s.blif", Options->out_file_prefix,
					Options->CircuitName);
		}
	}

	if (Options->NetFile == NULL ) {
		len = strlen(Options->CircuitName) + 5; /* circuit_name.net/0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->NetFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->NetFile, "%s.net", Options->CircuitName);
		} else {
			sprintf(Options->NetFile, "%s%s.net", Options->out_file_prefix,
					Options->CircuitName);
		}
	}

	if (Options->PlaceFile == NULL ) {
		len = strlen(Options->CircuitName) + 7; /* circuit_name.place/0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->PlaceFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->PlaceFile, "%s.place", Options->CircuitName);
		} else {
			sprintf(Options->PlaceFile, "%s%s.place", Options->out_file_prefix,
					Options->CircuitName);
		}
	}

	if (Options->RouteFile == NULL ) {
		len = strlen(Options->CircuitName) + 7; /* circuit_name.route/0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->RouteFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->RouteFile, "%s.route", Options->CircuitName);
		} else {
			sprintf(Options->RouteFile, "%s%s.route", Options->out_file_prefix,
					Options->CircuitName);
		}
	}
	if (Options->ActFile == NULL ) {
		len = strlen(Options->CircuitName) + 7; /* circuit_name.route/0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->ActFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->ActFile, "%s.act", Options->CircuitName);
		} else {
			sprintf(Options->ActFile, "%s%s.act", Options->out_file_prefix,
					Options->CircuitName);
		}
	}

	if (Options->PowerFile == NULL ) {
		len = strlen(Options->CircuitName) + 7; /* circuit_name.power\0*/
		if (Options->out_file_prefix != NULL ) {
			len += strlen(Options->out_file_prefix);
		}
		Options->PowerFile = (char*) my_calloc(len, sizeof(char));
		if (Options->out_file_prefix == NULL ) {
			sprintf(Options->PowerFile, "%s.power", Options->CircuitName);
		} else {
			sprintf(Options->ActFile, "%s%s.power", Options->out_file_prefix,
					Options->CircuitName);
		}
	}

	alloc_and_load_output_file_names(default_output_name);

	FileNameOpts->CircuitName = Options->CircuitName;
	FileNameOpts->ArchFile = Options->ArchFile;
	FileNameOpts->BlifFile = Options->BlifFile;
	FileNameOpts->NetFile = Options->NetFile;
	FileNameOpts->PlaceFile = Options->PlaceFile;
	FileNameOpts->RouteFile = Options->RouteFile;
	FileNameOpts->ActFile = Options->ActFile;
	FileNameOpts->PowerFile = Options->PowerFile;
	FileNameOpts->CmosTechFile = Options->CmosTechFile;
	FileNameOpts->out_file_prefix = Options->out_file_prefix;

	SetupOperation(*Options, Operation);
	SetupPlacerOpts(*Options, TimingEnabled, PlacerOpts);
	SetupAnnealSched(*Options, AnnealSched);
	SetupRouterOpts(*Options, TimingEnabled, RouterOpts);
	SetupPowerOpts(*Options, PowerOpts, Arch);
  
    /* Xifan TANG: FPGA-SPICE Tool suites Options Setup */
    SetupFpgaSpiceOpts(*Options, fpga_spice_opts, Arch);
    /* END */

	if (readArchFile == TRUE) {
		XmlReadArch(Options->ArchFile, TimingEnabled, Arch, &type_descriptors,
				&num_types);
	}

    VPRSetupArch(Arch, RoutingArch, Segments, swseg_patterns, user_models, library_models);

	SetupTiming(*Options, *Arch, TimingEnabled, *Operation, *PlacerOpts,
			*RouterOpts, Timing);
	SetupPackerOpts(*Options, TimingEnabled, *Arch, Options->NetFile,
			PackerOpts);

	/* init global variables */
	out_file_prefix = Options->out_file_prefix;
	grid_logic_tile_area = Arch->grid_logic_tile_area;
	ipin_mux_trans_size = Arch->ipin_mux_trans_size;

	/* Set seed for pseudo-random placement, default seed to 1 */
	PlacerOpts->seed = 1;
	if (Options->Count[OT_SEED]) {
		PlacerOpts->seed = Options->Seed;
	}
	my_srandom(PlacerOpts->seed);
    
    /* Build the complex block graph */
	vpr_printf(TIO_MESSAGE_INFO, "Building complex block graph.\n");
	alloc_and_load_all_pb_graphs(PowerOpts->do_power);

    /* Xifan Tang: Initialize the clb to clb directs */
    alloc_and_init_globals_clb_to_clb_directs(Arch->num_directs, Arch->Directs); 

	if (getEchoEnabled() && isEchoFileEnabled(E_ECHO_PB_GRAPH)) {
		echo_pb_graph(getEchoFileName(E_ECHO_PB_GRAPH));
	}

	*GraphPause = 1; /* DEFAULT */
	if (Options->Count[OT_AUTO]) {
		*GraphPause = Options->GraphPause;
	}
#ifdef NO_GRAPHICS
	*ShowGraphics = FALSE; /* DEFAULT */
#else /* NO_GRAPHICS */
	*ShowGraphics = TRUE; /* DEFAULT */
	if (Options->Count[OT_NODISP]) {
		*ShowGraphics = FALSE;
	}
#endif /* NO_GRAPHICS */

	if (getEchoEnabled() && isEchoFileEnabled(E_ECHO_ARCH)) {
		EchoArch(getEchoFileName(E_ECHO_ARCH), type_descriptors, num_types,
				Arch);
	}

}

static void SetupTiming(INP t_options Options, INP t_arch Arch,
		INP boolean TimingEnabled, INP enum e_operation Operation,
		INP struct s_placer_opts PlacerOpts,
		INP struct s_router_opts RouterOpts, OUTP t_timing_inf * Timing) {

	/* Don't do anything if they don't want timing */
	if (FALSE == TimingEnabled) {
		memset(Timing, 0, sizeof(t_timing_inf));
		Timing->timing_analysis_enabled = FALSE;
		return;
	}

	Timing->C_ipin_cblock = Arch.C_ipin_cblock;
	Timing->T_ipin_cblock = Arch.T_ipin_cblock;
	Timing->timing_analysis_enabled = TimingEnabled;

	/* If the user specified an SDC filename on the command line, look for specified_name.sdc, otherwise look for circuit_name.sdc*/
	if (Options.SDCFile == NULL ) {
		Timing->SDCFile = (char*) my_calloc(strlen(Options.CircuitName) + 5,
				sizeof(char)); /* circuit_name.sdc/0*/
		sprintf(Timing->SDCFile, "%s.sdc", Options.CircuitName);
	} else {
		Timing->SDCFile = (char*) my_strdup(Options.SDCFile);
	}
}

/* This loads up VPR's switch_inf data by combining the switches from 
 * the arch file with the special switches that VPR needs. */
static void SetupSwitches(INP t_arch Arch,
		INOUTP struct s_det_routing_arch *RoutingArch,
		INP struct s_switch_inf *ArchSwitches, INP int NumArchSwitches) {

	RoutingArch->num_switch = NumArchSwitches;
    
	/* Depends on RoutingArch->num_switch */
	RoutingArch->wire_to_ipin_switch = RoutingArch->num_switch;
	++RoutingArch->num_switch;

	/* Depends on RoutingArch->num_switch */
	RoutingArch->delayless_switch = RoutingArch->num_switch;
	RoutingArch->global_route_switch = RoutingArch->delayless_switch;
	++RoutingArch->num_switch;

	/* Alloc the list now that we know the final num_switch value */
	switch_inf = (struct s_switch_inf *) my_malloc(
			sizeof(struct s_switch_inf) * RoutingArch->num_switch);

	/* Copy the switch data from architecture file */
	memcpy(switch_inf, ArchSwitches,
			sizeof(struct s_switch_inf) * NumArchSwitches);

	/* Delayless switch for connecting sinks and sources with their pins. */
	switch_inf[RoutingArch->delayless_switch].buffered = TRUE;
	switch_inf[RoutingArch->delayless_switch].R = 0.;
	switch_inf[RoutingArch->delayless_switch].Cin = 0.;
	switch_inf[RoutingArch->delayless_switch].Cout = 0.;
	switch_inf[RoutingArch->delayless_switch].Tdel = 0.;
	switch_inf[RoutingArch->delayless_switch].power_buffer_type = POWER_BUFFER_TYPE_NONE;
	switch_inf[RoutingArch->delayless_switch].mux_trans_size = 0.;
    /* Xifan TANG: SPICE model support*/
	switch_inf[RoutingArch->delayless_switch].spice_model_name = NULL;
	switch_inf[RoutingArch->delayless_switch].spice_model = NULL;

	/* The wire to ipin switch for all types. Curently all types
	 * must share ipin switch. Some of the timing code would
	 * need to be changed otherwise. */
    /* Xifan TANG: Enhancement for connection blocks */
    if (0 == Arch.num_cb_switch) {
   	  switch_inf[RoutingArch->wire_to_ipin_switch].buffered = TRUE;
	  switch_inf[RoutingArch->wire_to_ipin_switch].R = 0.;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Cin = Arch.C_ipin_cblock;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Cout = 0.;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Tdel = Arch.T_ipin_cblock;
	  switch_inf[RoutingArch->wire_to_ipin_switch].power_buffer_type = POWER_BUFFER_TYPE_NONE;
	  switch_inf[RoutingArch->wire_to_ipin_switch].mux_trans_size = 0.;
	  switch_inf[RoutingArch->wire_to_ipin_switch].spice_model_name = NULL;
	  switch_inf[RoutingArch->wire_to_ipin_switch].spice_model = NULL;
    } else {
      /* Xifan TANG: Currently we only support 1 connection blocks switch defined.*/
      assert(1 == Arch.num_cb_switch);
	  memcpy(&switch_inf[RoutingArch->wire_to_ipin_switch], Arch.cb_switches,
			 sizeof(struct s_switch_inf) * Arch.num_cb_switch);
    }
}

/* This loads up VPR's switch_inf data by combining the switches from 
 * the arch file with the special switches that VPR needs. */
static void SetupSwitches_mrFPGA(INP t_arch Arch,
		INOUTP struct s_det_routing_arch *RoutingArch,
		INP struct s_switch_inf *ArchSwitches, INP int NumArchSwitches, INP t_segment_inf* segment_inf) {
    int i/*, switch_index*/;

	RoutingArch->num_switch = NumArchSwitches;

    /* mrFPGA : Xifan TANG*/
    /* Xifan TANG: only overwrite it when it is defined*/
    num_normal_switch = NumArchSwitches;
    if (is_mrFPGA && Arch.arch_mrfpga.is_opin_cblock_defined) {
        RoutingArch->opin_to_wire_switch = RoutingArch->num_switch;
        ++RoutingArch->num_switch;
    }
    /* end */

	/* Depends on RoutingArch->num_switch */
	RoutingArch->wire_to_ipin_switch = RoutingArch->num_switch;
	++RoutingArch->num_switch;

	/* Depends on RoutingArch->num_switch */
	RoutingArch->delayless_switch = RoutingArch->num_switch;
	RoutingArch->global_route_switch = RoutingArch->delayless_switch;
	++RoutingArch->num_switch;

    /*mrFPGA: Xifan TANG*/
    start_seg_switch = RoutingArch->num_switch;
    /* END */

	/* Alloc the list now that we know the final num_switch value */
	switch_inf = (struct s_switch_inf *) my_malloc(
			sizeof(struct s_switch_inf) * RoutingArch->num_switch);

	/* Copy the switch data from architecture file */
	memcpy(switch_inf, ArchSwitches,
			sizeof(struct s_switch_inf) * NumArchSwitches);

	/* Delayless switch for connecting sinks and sources with their pins. */
	switch_inf[RoutingArch->delayless_switch].buffered = TRUE;
	switch_inf[RoutingArch->delayless_switch].R = 0.;
	switch_inf[RoutingArch->delayless_switch].Cin = 0.;
	switch_inf[RoutingArch->delayless_switch].Cout = 0.;
	switch_inf[RoutingArch->delayless_switch].Tdel = 0.;
	switch_inf[RoutingArch->delayless_switch].power_buffer_type = POWER_BUFFER_TYPE_NONE;
	switch_inf[RoutingArch->delayless_switch].mux_trans_size = 0.;
    /* Xifan TANG: easy to identify internal built switch*/
    switch_inf[RoutingArch->delayless_switch].type = "buffer";
    switch_inf[RoutingArch->delayless_switch].name = "delayless_switch";
    /* end */
    /* Xifan TANG: SPICE model support*/
	switch_inf[RoutingArch->delayless_switch].spice_model_name = NULL;
	switch_inf[RoutingArch->delayless_switch].spice_model = NULL;
    /* END */

	/* The wire to ipin switch for all types. Curently all types
	 * must share ipin switch. Some of the timing code would
	 * need to be changed otherwise. */
    /* Xifan TANG: Enhancement for connection blocks */
    if (0 == Arch.num_cb_switch) {
   	  switch_inf[RoutingArch->wire_to_ipin_switch].buffered = TRUE;
	  switch_inf[RoutingArch->wire_to_ipin_switch].R = 0.;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Cin = Arch.C_ipin_cblock;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Cout = 0.;
	  switch_inf[RoutingArch->wire_to_ipin_switch].Tdel = Arch.T_ipin_cblock;
	  switch_inf[RoutingArch->wire_to_ipin_switch].power_buffer_type = POWER_BUFFER_TYPE_NONE;
	  switch_inf[RoutingArch->wire_to_ipin_switch].mux_trans_size = 0.;
      /* Xifan TANG: easy to identify internal built switch*/
      switch_inf[RoutingArch->wire_to_ipin_switch].type = "buffer";
      switch_inf[RoutingArch->wire_to_ipin_switch].name = "wire_to_ipin_switch";
      /* end */
	  switch_inf[RoutingArch->wire_to_ipin_switch].spice_model_name = NULL;
	  switch_inf[RoutingArch->wire_to_ipin_switch].spice_model = NULL;
    } else {
      /* Xifan TANG: Currently we only support 1 connection blocks switch defined.*/
      assert(1 == Arch.num_cb_switch);
	  memcpy(&switch_inf[RoutingArch->wire_to_ipin_switch], Arch.cb_switches,
			 sizeof(struct s_switch_inf) * Arch.num_cb_switch);
    }
    /* END */

    /* mrFPGA: Xifan TANG */
    if (is_mrFPGA && Arch.arch_mrfpga.is_opin_cblock_defined) {
      switch_inf[RoutingArch->opin_to_wire_switch].buffered = TRUE;
      switch_inf[RoutingArch->opin_to_wire_switch].R = Arch.arch_mrfpga.R_opin_cblock;
      switch_inf[RoutingArch->opin_to_wire_switch].Cin = 0.;
      switch_inf[RoutingArch->opin_to_wire_switch].Cout = 0.;
      switch_inf[RoutingArch->opin_to_wire_switch].Tdel = Arch.arch_mrfpga.T_opin_cblock;
      /* Xifan TANG: name should be specified!!!*/
	  switch_inf[RoutingArch->opin_to_wire_switch].power_buffer_type = POWER_BUFFER_TYPE_NONE;
	  switch_inf[RoutingArch->opin_to_wire_switch].mux_trans_size = 0.;
      switch_inf[RoutingArch->opin_to_wire_switch].type = "buffer";
      switch_inf[RoutingArch->opin_to_wire_switch].name = "mrFPGA_opin_switch";
	  switch_inf[RoutingArch->opin_to_wire_switch].spice_model_name = NULL;
	  switch_inf[RoutingArch->opin_to_wire_switch].spice_model = NULL;
    }

    for (i = 0; i < RoutingArch->num_segment; i++ ) {
      /* Xifan TANG: only overwrite it when it is defined*/
      if (is_mrFPGA && Arch.arch_mrfpga.is_opin_cblock_defined) {
        segment_inf[i].opin_switch = RoutingArch->opin_to_wire_switch;
      }
    }
    /* end */
}




/* Sets up routing structures. Since checks are already done, this
 * just copies values across */
static void SetupRoutingArch(INP t_arch Arch,
		OUTP struct s_det_routing_arch *RoutingArch) {

	RoutingArch->switch_block_type = Arch.SBType;
	RoutingArch->R_minW_nmos = Arch.R_minW_nmos;
	RoutingArch->R_minW_pmos = Arch.R_minW_pmos;
	RoutingArch->Fs = Arch.Fs;
	RoutingArch->directionality = BI_DIRECTIONAL;
	if (Arch.Segments)
		RoutingArch->directionality = Arch.Segments[0].directionality;
}

static void SetupRouterOpts(INP t_options Options, INP boolean TimingEnabled,
		OUTP struct s_router_opts *RouterOpts) {
	RouterOpts->astar_fac = 1.2; /* DEFAULT */
	if (Options.Count[OT_ASTAR_FAC]) {
		RouterOpts->astar_fac = Options.astar_fac;
	}

	RouterOpts->bb_factor = 3; /* DEFAULT */
	if (Options.Count[OT_FAST]) {
		RouterOpts->bb_factor = 0; /* DEFAULT */
	}
	if (Options.Count[OT_BB_FACTOR]) {
		RouterOpts->bb_factor = Options.bb_factor;
	}

	RouterOpts->criticality_exp = 1.0; /* DEFAULT */
	if (Options.Count[OT_CRITICALITY_EXP]) {
		RouterOpts->criticality_exp = Options.criticality_exp;
	}

	RouterOpts->max_criticality = 0.99; /* DEFAULT */
	if (Options.Count[OT_MAX_CRITICALITY]) {
		RouterOpts->max_criticality = Options.max_criticality;
	}

	RouterOpts->max_router_iterations = 50; /* DEFAULT */
	if (Options.Count[OT_FAST]) {
		RouterOpts->max_router_iterations = 10;
	}
	if (Options.Count[OT_MAX_ROUTER_ITERATIONS]) {
		RouterOpts->max_router_iterations = Options.max_router_iterations;
	}

	RouterOpts->pres_fac_mult = 1.3; /* DEFAULT */
	if (Options.Count[OT_PRES_FAC_MULT]) {
		RouterOpts->pres_fac_mult = Options.pres_fac_mult;
	}

	RouterOpts->route_type = DETAILED; /* DEFAULT */
	if (Options.Count[OT_ROUTE_TYPE]) {
		RouterOpts->route_type = Options.RouteType;
	}

	RouterOpts->full_stats = FALSE; /* DEFAULT */
	if (Options.Count[OT_FULL_STATS]) {
		RouterOpts->full_stats = TRUE;
	}

	RouterOpts->verify_binary_search = FALSE; /* DEFAULT */
	if (Options.Count[OT_VERIFY_BINARY_SEARCH]) {
		RouterOpts->verify_binary_search = TRUE;
	}

	/* Depends on RouteOpts->route_type */
	RouterOpts->router_algorithm = NO_TIMING; /* DEFAULT */
	if (TimingEnabled) {
		RouterOpts->router_algorithm = TIMING_DRIVEN; /* DEFAULT */
	}
	if (GLOBAL == RouterOpts->route_type) {
		RouterOpts->router_algorithm = NO_TIMING; /* DEFAULT */
	}
	if (Options.Count[OT_ROUTER_ALGORITHM]) {
		RouterOpts->router_algorithm = Options.RouterAlgorithm;
	}

	RouterOpts->fixed_channel_width = NO_FIXED_CHANNEL_WIDTH; /* DEFAULT */
	if (Options.Count[OT_ROUTE_CHAN_WIDTH]) {
		RouterOpts->fixed_channel_width = Options.RouteChanWidth;
	}

    /* mrFPGA: Xifan TANG */
    is_show_sram = FALSE;
    if (Options.Count[OT_SHOW_SRAM]) {
      is_show_sram = TRUE;
    }
    is_show_pass_trans = FALSE;
    if (Options.Count[OT_SHOW_PASS_TRANS]) {
      is_show_pass_trans = TRUE;
    } 
    /* END */

	/* Depends on RouterOpts->router_algorithm */
	RouterOpts->initial_pres_fac = 0.5; /* DEFAULT */
	if (NO_TIMING == RouterOpts->router_algorithm || Options.Count[OT_FAST]) {
		RouterOpts->initial_pres_fac = 10000.0; /* DEFAULT */
	}
	if (Options.Count[OT_INITIAL_PRES_FAC]) {
		RouterOpts->initial_pres_fac = Options.initial_pres_fac;
	}

	/* Depends on RouterOpts->router_algorithm */
	RouterOpts->base_cost_type = DELAY_NORMALIZED; /* DEFAULT */
	if (BREADTH_FIRST == RouterOpts->router_algorithm) {
		RouterOpts->base_cost_type = DEMAND_ONLY; /* DEFAULT */
	}
	if (NO_TIMING == RouterOpts->router_algorithm) {
		RouterOpts->base_cost_type = DEMAND_ONLY; /* DEFAULT */
	}
	if (Options.Count[OT_BASE_COST_TYPE]) {
		RouterOpts->base_cost_type = Options.base_cost_type;
	}

	/* Depends on RouterOpts->router_algorithm */
	RouterOpts->first_iter_pres_fac = 0.5; /* DEFAULT */
	if (BREADTH_FIRST == RouterOpts->router_algorithm) {
		RouterOpts->first_iter_pres_fac = 0.0; /* DEFAULT */
	}
	if (NO_TIMING == RouterOpts->router_algorithm || Options.Count[OT_FAST]) {
		RouterOpts->first_iter_pres_fac = 10000.0; /* DEFAULT */
	}
	if (Options.Count[OT_FIRST_ITER_PRES_FAC]) {
		RouterOpts->first_iter_pres_fac = Options.first_iter_pres_fac;
	}

	/* Depends on RouterOpts->router_algorithm */
	RouterOpts->acc_fac = 1.0;
	if (BREADTH_FIRST == RouterOpts->router_algorithm) {
		RouterOpts->acc_fac = 0.2;
	}
	if (Options.Count[OT_ACC_FAC]) {
		RouterOpts->acc_fac = Options.acc_fac;
	}

	/* Depends on RouterOpts->route_type */
	RouterOpts->bend_cost = 0.0; /* DEFAULT */
	if (GLOBAL == RouterOpts->route_type) {
		RouterOpts->bend_cost = 1.0; /* DEFAULT */
	}
	if (Options.Count[OT_BEND_COST]) {
		RouterOpts->bend_cost = Options.bend_cost;
	}

	RouterOpts->doRouting = FALSE;
	if (Options.Count[OT_ROUTE]) {
		RouterOpts->doRouting = TRUE;
	} else if (!Options.Count[OT_PACK] && !Options.Count[OT_PLACE]
			&& !Options.Count[OT_ROUTE]) {
		if (!Options.Count[OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY])
			RouterOpts->doRouting = TRUE;
	}

}

static void SetupAnnealSched(INP t_options Options,
		OUTP struct s_annealing_sched *AnnealSched) {
	AnnealSched->alpha_t = 0.8; /* DEFAULT */
	if (Options.Count[OT_ALPHA_T]) {
		AnnealSched->alpha_t = Options.PlaceAlphaT;
	}
	if (AnnealSched->alpha_t >= 1 || AnnealSched->alpha_t <= 0) {
		vpr_printf(TIO_MESSAGE_ERROR,
				"alpha_t must be between 0 and 1 exclusive.\n");
		exit(1);
	}
	AnnealSched->exit_t = 0.01; /* DEFAULT */
	if (Options.Count[OT_EXIT_T]) {
		AnnealSched->exit_t = Options.PlaceExitT;
	}
	if (AnnealSched->exit_t <= 0) {
		vpr_printf(TIO_MESSAGE_ERROR, "exit_t must be greater than 0.\n");
		exit(1);
	}
	AnnealSched->init_t = 100.0; /* DEFAULT */
	if (Options.Count[OT_INIT_T]) {
		AnnealSched->init_t = Options.PlaceInitT;
	}
	if (AnnealSched->init_t <= 0) {
		vpr_printf(TIO_MESSAGE_ERROR, "init_t must be greater than 0.\n");
		exit(1);
	}
	if (AnnealSched->init_t < AnnealSched->exit_t) {
		vpr_printf(TIO_MESSAGE_ERROR,
				"init_t must be greater or equal to than exit_t.\n");
		exit(1);
	}
	AnnealSched->inner_num = 1.0; /* DEFAULT */
	if (Options.Count[OT_INNER_NUM]) {
		AnnealSched->inner_num = Options.PlaceInnerNum;
	}
	if (AnnealSched->inner_num <= 0) {
		vpr_printf(TIO_MESSAGE_ERROR, "init_t must be greater than 0.\n");
		exit(1);
	}
	AnnealSched->type = AUTO_SCHED; /* DEFAULT */
	if ((Options.Count[OT_ALPHA_T]) || (Options.Count[OT_EXIT_T])
			|| (Options.Count[OT_INIT_T])) {
		AnnealSched->type = USER_SCHED;
	}
}

/* Sets up the s_packer_opts structure baesd on users inputs and on the architecture specified.  
 * Error checking, such as checking for conflicting params is assumed to be done beforehand 
 */
void SetupPackerOpts(INP t_options Options, INP boolean TimingEnabled,
		INP t_arch Arch, INP char *net_file,
		OUTP struct s_packer_opts *PackerOpts) {

	if (Arch.clb_grid.IsAuto) {
		PackerOpts->aspect = Arch.clb_grid.Aspect;
	} else {
		PackerOpts->aspect = (float) Arch.clb_grid.H / (float) Arch.clb_grid.W;
	}
	PackerOpts->output_file = net_file;

	PackerOpts->blif_file_name = Options.BlifFile;

	PackerOpts->doPacking = FALSE; /* DEFAULT */
	if (Options.Count[OT_PACK]) {
		PackerOpts->doPacking = TRUE;
	} else if (!Options.Count[OT_PACK] && !Options.Count[OT_PLACE]
			&& !Options.Count[OT_ROUTE]) {
		if (!Options.Count[OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY])
			PackerOpts->doPacking = TRUE;
	}

	PackerOpts->global_clocks = TRUE; /* DEFAULT */
	if (Options.Count[OT_GLOBAL_CLOCKS]) {
		PackerOpts->global_clocks = Options.global_clocks;
	}

	PackerOpts->hill_climbing_flag = FALSE; /* DEFAULT */
	if (Options.Count[OT_HILL_CLIMBING_FLAG]) {
		PackerOpts->hill_climbing_flag = Options.hill_climbing_flag;
	}

	PackerOpts->sweep_hanging_nets_and_inputs = TRUE;
	if (Options.Count[OT_SWEEP_HANGING_NETS_AND_INPUTS]) {
		PackerOpts->sweep_hanging_nets_and_inputs =
				Options.sweep_hanging_nets_and_inputs;
	}

	PackerOpts->skip_clustering = FALSE; /* DEFAULT */
	if (Options.Count[OT_SKIP_CLUSTERING]) {
		PackerOpts->skip_clustering = TRUE;
	}
	PackerOpts->allow_unrelated_clustering = TRUE; /* DEFAULT */
	if (Options.Count[OT_ALLOW_UNRELATED_CLUSTERING]) {
		PackerOpts->allow_unrelated_clustering =
				Options.allow_unrelated_clustering;
	}
	PackerOpts->allow_early_exit = FALSE; /* DEFAULT */
	if (Options.Count[OT_ALLOW_EARLY_EXIT]) {
		PackerOpts->allow_early_exit = Options.allow_early_exit;
	}
	PackerOpts->connection_driven = TRUE; /* DEFAULT */
	if (Options.Count[OT_CONNECTION_DRIVEN_CLUSTERING]) {
		PackerOpts->connection_driven = Options.connection_driven;
	}

	PackerOpts->timing_driven = TimingEnabled; /* DEFAULT */
	if (Options.Count[OT_TIMING_DRIVEN_CLUSTERING]) {
		PackerOpts->timing_driven = Options.timing_driven;
	}
	PackerOpts->cluster_seed_type = (
			TimingEnabled ? VPACK_TIMING : VPACK_MAX_INPUTS); /* DEFAULT */
	if (Options.Count[OT_CLUSTER_SEED]) {
		PackerOpts->cluster_seed_type = Options.cluster_seed_type;
	}
	PackerOpts->alpha = 0.75; /* DEFAULT */
	if (Options.Count[OT_ALPHA_CLUSTERING]) {
		PackerOpts->alpha = Options.alpha;
	}
	PackerOpts->beta = 0.9; /* DEFAULT */
	if (Options.Count[OT_BETA_CLUSTERING]) {
		PackerOpts->beta = Options.beta;
	}

	/* never recomputer timing */
	PackerOpts->recompute_timing_after = MAX_SHORT; /* DEFAULT */
	if (Options.Count[OT_RECOMPUTE_TIMING_AFTER]) {
		PackerOpts->recompute_timing_after = Options.recompute_timing_after;
	}
	PackerOpts->block_delay = 0; /* DEFAULT */
	if (Options.Count[OT_CLUSTER_BLOCK_DELAY]) {
		PackerOpts->block_delay = Options.block_delay;
	}
	PackerOpts->intra_cluster_net_delay = 0; /* DEFAULT */
	if (Options.Count[OT_INTRA_CLUSTER_NET_DELAY]) {
		PackerOpts->intra_cluster_net_delay = Options.intra_cluster_net_delay;
	}
	PackerOpts->inter_cluster_net_delay = 1.0; /* DEFAULT */
	PackerOpts->auto_compute_inter_cluster_net_delay = TRUE;
	if (Options.Count[OT_INTER_CLUSTER_NET_DELAY]) {
		PackerOpts->inter_cluster_net_delay = Options.inter_cluster_net_delay;
		PackerOpts->auto_compute_inter_cluster_net_delay = FALSE;
	}

	PackerOpts->packer_algorithm = PACK_GREEDY; /* DEFAULT */
	if (Options.Count[OT_PACKER_ALGORITHM]) {
		PackerOpts->packer_algorithm = Options.packer_algorithm;
	}
  
    /* Xifan TANG: PACK_CLB_PIN_REMAP */ 
	PackerOpts->pack_clb_pin_remap = FALSE; /* DEFAULT */
	if (Options.Count[OT_PACK_CLB_PIN_REMAP]) {
		PackerOpts->pack_clb_pin_remap = TRUE;
	}
   
}

/* Sets up the s_placer_opts structure based on users input. Error checking,
 * such as checking for conflicting params is assumed to be done beforehand */
static void SetupPlacerOpts(INP t_options Options, INP boolean TimingEnabled,
		OUTP struct s_placer_opts *PlacerOpts) {
	PlacerOpts->block_dist = 1; /* DEFAULT */
	if (Options.Count[OT_BLOCK_DIST]) {
		PlacerOpts->block_dist = Options.block_dist;
	}

	PlacerOpts->inner_loop_recompute_divider = 0; /* DEFAULT */
	if (Options.Count[OT_INNER_LOOP_RECOMPUTE_DIVIDER]) {
		PlacerOpts->inner_loop_recompute_divider =
				Options.inner_loop_recompute_divider;
	}

	PlacerOpts->place_cost_exp = 1.; /* DEFAULT */
	if (Options.Count[OT_PLACE_COST_EXP]) {
		PlacerOpts->place_cost_exp = Options.place_cost_exp;
	}

	PlacerOpts->td_place_exp_first = 1.; /* DEFAULT */
	if (Options.Count[OT_TD_PLACE_EXP_FIRST]) {
		PlacerOpts->td_place_exp_first = Options.place_exp_first;
	}

	PlacerOpts->td_place_exp_last = 8.; /* DEFAULT */
	if (Options.Count[OT_TD_PLACE_EXP_LAST]) {
		PlacerOpts->td_place_exp_last = Options.place_exp_last;
	}

	PlacerOpts->place_algorithm = BOUNDING_BOX_PLACE; /* DEFAULT */
	if (TimingEnabled) {
		PlacerOpts->place_algorithm = PATH_TIMING_DRIVEN_PLACE; /* DEFAULT */
	}
	if (Options.Count[OT_PLACE_ALGORITHM]) {
		PlacerOpts->place_algorithm = Options.PlaceAlgorithm;
	}

	PlacerOpts->pad_loc_file = NULL; /* DEFAULT */
	if (Options.Count[OT_FIX_PINS]) {
		if (Options.PinFile) {
			PlacerOpts->pad_loc_file = my_strdup(Options.PinFile);
		}
	}

	PlacerOpts->pad_loc_type = FREE; /* DEFAULT */
	if (Options.Count[OT_FIX_PINS]) {
		PlacerOpts->pad_loc_type = (Options.PinFile ? USER : RANDOM);
	}

	PlacerOpts->place_chan_width = 100; /* DEFAULT */
	if (Options.Count[OT_PLACE_CHAN_WIDTH]) {
		PlacerOpts->place_chan_width = Options.PlaceChanWidth;
	}

	PlacerOpts->recompute_crit_iter = 1; /* DEFAULT */
	if (Options.Count[OT_RECOMPUTE_CRIT_ITER]) {
		PlacerOpts->recompute_crit_iter = Options.RecomputeCritIter;
	}

	PlacerOpts->timing_tradeoff = 0.5; /* DEFAULT */
	if (Options.Count[OT_TIMING_TRADEOFF]) {
		PlacerOpts->timing_tradeoff = Options.PlaceTimingTradeoff;
	}
    
    /* Xifan TANG : PLACE_CLB_PIN_REMAP */
	PlacerOpts->place_clb_pin_remap = FALSE; /* DEFAULT */
	if (Options.Count[OT_PLACE_CLB_PIN_REMAP]) {
		PlacerOpts->place_clb_pin_remap = TRUE;
	}
    /* END */

	/* Depends on PlacerOpts->place_algorithm */
	PlacerOpts->enable_timing_computations = FALSE; /* DEFAULT */
	if ((PlacerOpts->place_algorithm == PATH_TIMING_DRIVEN_PLACE)
			|| (PlacerOpts->place_algorithm == NET_TIMING_DRIVEN_PLACE)) {
		PlacerOpts->enable_timing_computations = TRUE; /* DEFAULT */
	}
	if (Options.Count[OT_ENABLE_TIMING_COMPUTATIONS]) {
		PlacerOpts->enable_timing_computations = Options.ShowPlaceTiming;
	}

	PlacerOpts->place_freq = PLACE_ONCE; /* DEFAULT */
	if ((Options.Count[OT_ROUTE_CHAN_WIDTH])
			|| (Options.Count[OT_PLACE_CHAN_WIDTH])) {
		PlacerOpts->place_freq = PLACE_ONCE;
	}

	PlacerOpts->doPlacement = FALSE; /* DEFAULT */
	if (Options.Count[OT_PLACE]) {
		PlacerOpts->doPlacement = TRUE;
	} else if (!Options.Count[OT_PACK] && !Options.Count[OT_PLACE]
			&& !Options.Count[OT_ROUTE]) {
		if (!Options.Count[OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY])
			PlacerOpts->doPlacement = TRUE;
	}
	if (PlacerOpts->doPlacement == FALSE) {
		PlacerOpts->place_freq = PLACE_NEVER;
	}

}

static void SetupOperation(INP t_options Options,
		OUTP enum e_operation *Operation) {
	*Operation = RUN_FLOW; /* DEFAULT */
	if (Options.Count[OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY]) {
		*Operation = TIMING_ANALYSIS_ONLY;
	}
}

static void SetupPowerOpts(t_options Options, t_power_opts *power_opts,
		t_arch * Arch) {

	if (Options.Count[OT_POWER]) {
		power_opts->do_power = TRUE;
	} else {
		power_opts->do_power = FALSE;
	}

	if (power_opts->do_power) {
		Arch->power = (t_power_arch*) my_malloc(sizeof(t_power_arch));
		Arch->clocks = (t_clock_arch*) my_malloc(sizeof(t_clock_arch));
		g_clock_arch = Arch->clocks;
	} else {
		Arch->power = NULL;
		Arch->clocks = NULL;
		g_clock_arch = NULL;
	}

}

/* Setup the SPICE Options:*/
static void SetupSpiceOpts(t_options Options, 
                           t_spice_opts* spice_opts,
                           t_arch* arch) {
  /* Initialize */  
  spice_opts->do_spice = FALSE;
  spice_opts->fpga_spice_print_top_testbench = FALSE;
  spice_opts->fpga_spice_print_pb_mux_testbench = FALSE;
  spice_opts->fpga_spice_print_cb_mux_testbench = FALSE;
  spice_opts->fpga_spice_print_sb_mux_testbench = FALSE;
  spice_opts->fpga_spice_print_cb_testbench = FALSE;
  spice_opts->fpga_spice_print_sb_testbench = FALSE;
  spice_opts->fpga_spice_print_lut_testbench = FALSE;
  spice_opts->fpga_spice_print_hardlogic_testbench = FALSE;
  spice_opts->fpga_spice_print_io_testbench = FALSE;
  spice_opts->fpga_spice_print_grid_testbench = FALSE;
  spice_opts->fpga_spice_leakage_only = FALSE;
  spice_opts->fpga_spice_parasitic_net_estimation = TRUE;
  spice_opts->fpga_spice_testbench_load_extraction = TRUE;

  /* Turn on the spice option if it is selected*/
  if (Options.Count[OT_FPGA_SPICE]) {
    spice_opts->do_spice = TRUE;
    spice_opts->spice_dir = my_strdup(Options.spice_dir);
    /* TODO: this could be more flexible*/
    spice_opts->include_dir = "include/";
    spice_opts->subckt_dir = "subckt/";
    if (Options.Count[OT_FPGA_SPICE_PRINT_TOP_TESTBENCH]) {
      spice_opts->fpga_spice_print_top_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_PB_MUX_TESTBENCH]) {
      spice_opts->fpga_spice_print_pb_mux_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_CB_MUX_TESTBENCH]) {
      spice_opts->fpga_spice_print_cb_mux_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_SB_MUX_TESTBENCH]) {
      spice_opts->fpga_spice_print_sb_mux_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_CB_TESTBENCH]) {
      spice_opts->fpga_spice_print_cb_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_SB_TESTBENCH]) {
      spice_opts->fpga_spice_print_sb_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_GRID_TESTBENCH]) {
      spice_opts->fpga_spice_print_grid_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_LUT_TESTBENCH]) {
      spice_opts->fpga_spice_print_lut_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_HARDLOGIC_TESTBENCH]) {
      spice_opts->fpga_spice_print_hardlogic_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PRINT_IO_TESTBENCH]) {
      spice_opts->fpga_spice_print_io_testbench = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_LEAKAGE_ONLY]) {
      spice_opts->fpga_spice_leakage_only = TRUE;
    }
    if (Options.Count[OT_FPGA_SPICE_PARASITIC_NET_ESTIMATION]) {
      spice_opts->fpga_spice_parasitic_net_estimation = Options.fpga_spice_parasitic_net_estimation;
    }
    if (Options.Count[OT_FPGA_SPICE_TESTBENCH_LOAD_EXTRACTION]) {
      spice_opts->fpga_spice_testbench_load_extraction = Options.fpga_spice_testbench_load_extraction;
    }
  }
  /* Set default options */
  if ((TRUE == spice_opts->do_spice)
    &&(FALSE == spice_opts->fpga_spice_print_top_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_grid_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_pb_mux_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_cb_mux_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_sb_mux_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_cb_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_sb_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_lut_testbench)
    &&(FALSE == spice_opts->fpga_spice_print_hardlogic_testbench)) {
    spice_opts->fpga_spice_print_pb_mux_testbench = TRUE;
    spice_opts->fpga_spice_print_cb_mux_testbench = TRUE;
    spice_opts->fpga_spice_print_sb_mux_testbench = TRUE;
    spice_opts->fpga_spice_print_lut_testbench = TRUE;
    spice_opts->fpga_spice_print_hardlogic_testbench = TRUE;
  }

  /* Assign the number of mt in SPICE simulation */
  spice_opts->fpga_spice_sim_multi_thread_num = 8;
  if (Options.Count[OT_FPGA_SPICE_SIM_MT_NUM]) { 
    spice_opts->fpga_spice_sim_multi_thread_num = Options.fpga_spice_sim_mt_num;
  }

  /* Assign path of SPICE simulator */
  spice_opts->simulator_path = NULL;
  if (Options.Count[OT_FPGA_SPICE_SIMULATOR_PATH]) {
    spice_opts->simulator_path = my_strdup(Options.fpga_spice_simulator_path);
  }

  /* If spice option is selected*/
  arch->read_xml_spice = spice_opts->do_spice;
  arch->spice = (t_spice*)my_malloc(sizeof(t_spice));

  return;
}

/*Xifan TANG: Synthesizable Verilog Dumping */
static void SetupSynVerilogOpts(t_options Options, 
                                t_syn_verilog_opts* syn_verilog_opts,
                                t_arch* arch) {

  /* Initialize */  
  syn_verilog_opts->dump_syn_verilog = FALSE;
  syn_verilog_opts->syn_verilog_dump_dir = NULL;
  syn_verilog_opts->print_top_testbench = FALSE;
  syn_verilog_opts->print_autocheck_top_testbench = FALSE;
  syn_verilog_opts->reference_verilog_benchmark_file = NULL;
  syn_verilog_opts->print_input_blif_testbench = FALSE;
  syn_verilog_opts->include_timing = FALSE;
  syn_verilog_opts->include_signal_init = FALSE;
  syn_verilog_opts->print_modelsim_autodeck = FALSE;
  syn_verilog_opts->print_formal_verification_top_netlist= FALSE;
  syn_verilog_opts->modelsim_ini_path = NULL;
  syn_verilog_opts->print_user_defined_template = FALSE;
  syn_verilog_opts->print_report_timing_tcl = FALSE;
  syn_verilog_opts->print_sdc_pnr = FALSE;
  syn_verilog_opts->print_sdc_analysis = FALSE;
  syn_verilog_opts->include_icarus_simulator = FALSE;

  /* Turn on Syn_verilog options */
  if (Options.Count[OT_FPGA_VERILOG_SYN]) {
    syn_verilog_opts->dump_syn_verilog = TRUE;
  } else {
    return;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_DIR]) {
    syn_verilog_opts->syn_verilog_dump_dir = my_strdup(Options.fpga_syn_verilog_dir);
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_TOP_TESTBENCH]) {
    syn_verilog_opts->print_top_testbench = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_AUTOCHECK_TOP_TESTBENCH]) {
    syn_verilog_opts->print_autocheck_top_testbench = TRUE;
    syn_verilog_opts->reference_verilog_benchmark_file = my_strdup(Options.fpga_verilog_reference_benchmark_file);
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_INPUT_BLIF_TESTBENCH]) {
    syn_verilog_opts->print_input_blif_testbench = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_FORMAL_VERIFICATION_TOP_NETLIST]) {
    syn_verilog_opts->print_formal_verification_top_netlist = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_INCLUDE_TIMING]) {
    syn_verilog_opts->include_timing = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_INCLUDE_SIGNAL_INIT]) {
    syn_verilog_opts->include_signal_init = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_INCLUDE_ICARUS_SIMULATOR]) {
    syn_verilog_opts->include_icarus_simulator = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_MODELSIM_AUTODECK]) {
    syn_verilog_opts->print_modelsim_autodeck = TRUE;
    syn_verilog_opts->modelsim_ini_path = my_strdup(Options.fpga_verilog_modelsim_ini_path);
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_USER_DEFINED_TEMPLATE]) {
    syn_verilog_opts->print_user_defined_template = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_REPORT_TIMING_TCL]) {
    syn_verilog_opts->print_report_timing_tcl = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_REPORT_TIMING_RPT_PATH]) {
    syn_verilog_opts->report_timing_path = my_strdup(Options.fpga_verilog_report_timing_path);
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_SDC_PNR]) {
    syn_verilog_opts->print_sdc_pnr = TRUE;
  }

  if (Options.Count[OT_FPGA_VERILOG_SYN_PRINT_SDC_ANALYSIS]) {
    syn_verilog_opts->print_sdc_analysis = TRUE;
  }

  /* SynVerilog needs the input from spice modeling */
  if (FALSE == arch->read_xml_spice) {
    arch->read_xml_spice = syn_verilog_opts->dump_syn_verilog;
    arch->spice = (t_spice*)my_malloc(sizeof(t_spice));
  }

  return;
}

/*Xifan TANG: Bitstream Generator */
static void SetupBitstreamGenOpts(t_options Options, 
                                  t_bitstream_gen_opts* bitstream_gen_opts,
                                  t_arch* arch) {

  /* Initialize */  
  bitstream_gen_opts->gen_bitstream = FALSE;
  bitstream_gen_opts->bitstream_output_file = NULL;

  /* Turn on Bitstream Generator options */
  if (Options.Count[OT_FPGA_BITSTREAM_GENERATOR]) {
    bitstream_gen_opts->gen_bitstream = TRUE;
  } else {
    return;
  }

  if (Options.Count[OT_FPGA_BITSTREAM_OUTPUT_FILE]) {
    bitstream_gen_opts->bitstream_output_file = my_strdup(Options.fpga_bitstream_file);
  }

  /* SynVerilog needs the input from spice modeling */
  if (FALSE == arch->read_xml_spice) {
    arch->read_xml_spice = bitstream_gen_opts->gen_bitstream;
    arch->spice = (t_spice*)my_malloc(sizeof(t_spice));
  }

  return;
}

static void SetupFpgaSpiceOpts(t_options Options, 
                               t_fpga_spice_opts* fpga_spice_opts,
                               t_arch* Arch) {
  /* Xifan TANG: SPICE Support*/
  SetupSpiceOpts(Options, &(fpga_spice_opts->SpiceOpts), Arch);   

  /* Xifan TANG: Synthesizable Verilog Dumping*/
  SetupSynVerilogOpts(Options, &(fpga_spice_opts->SynVerilogOpts), Arch);   

  /* Xifan TANG: Bitstream generator */
  SetupBitstreamGenOpts(Options, &(fpga_spice_opts->BitstreamGenOpts), Arch);

  /* Decide if we need to rename illegal port names */
  fpga_spice_opts->rename_illegal_port = FALSE;
  if (Options.Count[OT_FPGA_X2P_RENAME_ILLEGAL_PORT]) {
    fpga_spice_opts->rename_illegal_port = TRUE;
  }

  /* Assign the weight of signal density */
  fpga_spice_opts->signal_density_weight = 1.;
  if (Options.Count[OT_FPGA_X2P_SIGNAL_DENSITY_WEIGHT]) { 
    fpga_spice_opts->signal_density_weight = Options.fpga_spice_signal_density_weight;
  }

  /* Assign the weight of signal density */
  fpga_spice_opts->sim_window_size = 0.5;
  if (Options.Count[OT_FPGA_X2P_SIM_WINDOW_SIZE]) { 
    fpga_spice_opts->sim_window_size = Options.fpga_spice_sim_window_size;
  }

  /* Check if user wants to use a compact routing hierarchy */
  fpga_spice_opts->compact_routing_hierarchy = FALSE;
  if (Options.Count[OT_FPGA_X2P_COMPACT_ROUTING_HIERARCHY]) { 
    fpga_spice_opts->compact_routing_hierarchy = TRUE;
  }

  /* Decide if we need to do FPGA-SPICE */
  fpga_spice_opts->do_fpga_spice = FALSE;
  if (( TRUE == fpga_spice_opts->SpiceOpts.do_spice)
     ||(TRUE == fpga_spice_opts->SynVerilogOpts.dump_syn_verilog)
     ||(TRUE == fpga_spice_opts->BitstreamGenOpts.gen_bitstream)) {
    fpga_spice_opts->do_fpga_spice = TRUE;
  }

  /* Decide if we need to read activity file */
  fpga_spice_opts->read_act_file = FALSE;
  if (( TRUE == fpga_spice_opts->SpiceOpts.do_spice)
     ||(TRUE == fpga_spice_opts->SynVerilogOpts.dump_syn_verilog)) {
    fpga_spice_opts->read_act_file = TRUE;
  }

  return;
}

/* Initialize the global variables for clb to clb directs */
void alloc_and_init_globals_clb_to_clb_directs(int num_directs, 
                                               t_direct_inf* directs) {
  num_clb2clb_directs = num_directs;
  clb2clb_direct = alloc_and_load_clb_to_clb_directs(directs, num_directs);

  return;
} 

/* mrFPGA : Reshaped by Xifan TANG */
static void set_max_pins_per_side() {
  int i, j, p, q;
  max_pins_per_side = 0;

  for (p = 0; p < num_types; ++p) {
    for (q = 0; q < type_descriptors[p].height; ++q) {
      /* Xifan TANG: Skip NULL pointor*/      
      if (EMPTY_TYPE == &(type_descriptors[p])) {
        continue;
      }
      for (i = 0; i <= 3; i++) {
        int sum = 0;
        for (j = 0; j < type_descriptors[p].num_pins; j++) {
          if (1 == type_descriptors[p].pinloc[q][i][j]) {
            type_descriptors[p].pin_index_per_side[j] = sum;
            type_descriptors[p].pin_ptc_to_side[j] = i;
            sum++; 
          }
        }
        if (IO_TYPE != &(type_descriptors[p])) {
           max_pins_per_side = std::max(sum, max_pins_per_side);
        }
      }
    }
  }
  return;
}

static void setup_junction_switch(struct s_det_routing_arch *det_routing_arch) {
    int i;
    if (is_wire_buffer) {
        wire_buffer_inf.R += memristor_inf.R;
    }
    for (i = 0; i < num_normal_switch; i++) {
        if ( switch_inf[i].buffered ) {
            switch_inf[i].R += memristor_inf.R;
        } else {
            switch_inf[i].R += 2.0 * memristor_inf.R;
        }
        switch_inf[i].Tdel += memristor_inf.R * (0.5 * memristor_inf.C + switch_inf[i].Cin);
        switch_inf[i].Tdel += 2.0 * memristor_inf.Tdel;
    }
    switch_inf[det_routing_arch->wire_to_ipin_switch].Tdel += memristor_inf.R * (0.5 * memristor_inf.C + switch_inf[i].Cin) + memristor_inf.Tdel;
    if ( is_mrFPGA ) {
        switch_inf[det_routing_arch->opin_to_wire_switch].R += memristor_inf.R;
        switch_inf[det_routing_arch->opin_to_wire_switch].Tdel += memristor_inf.Tdel;
    }
}

static void add_wire_to_switch(struct s_det_routing_arch *det_routing_arch) {
    int i;
    for (i = 0; i < num_normal_switch; i++) {
        switch_inf[i].Tdel += switch_inf[i].R * Cseg_global + 0.5 * Rseg_global * Cseg_global;
        switch_inf[i].R += Rseg_global;
    }
}

static void hack_switch_to_rram(struct s_det_routing_arch *det_routing_arch) {
    int i;
    if(rram_pass_tran_value > 0.01) {
        for (i = 0; i < num_normal_switch; i++) {
            if(switch_inf[i].buffered) {
                switch_inf[i].R = switch_inf[i].R / 2. + rram_pass_tran_value;
            } else {
                switch_inf[i].R = rram_pass_tran_value;
            }
        }
    }
}

/* end */

