#include <string.h>
#include <assert.h>
#include "util.h"
#include "arch_types.h"
#include "ReadLine.h"
#include "ezxml.h"
#include "read_xml_util.h"

/* SPICE Support headers*/
#include "read_xml_spice_util.h"
#include "read_xml_spice.h"

/*********** Subroutines Declaration (only called in this source file) **********/
static void ProcessSpiceMeasParams(ezxml_t Parent,
                                   t_spice_meas_params* meas_params);

static void ProcessSpiceStimulateParamsRiseFall(ezxml_t Parent,
                                                float* rise_param,
                                                float* fall_param,
                                                enum e_spice_accuracy_type* rise_type,
                                                enum e_spice_accuracy_type* fall_type);

static void ProcessSpiceStimulateParams(ezxml_t Parent,
                                        t_spice_stimulate_params* stimulate_params);

static void ProcessSpiceParams(ezxml_t Parent,
                               t_spice_params* spice_params);

static void ProcessSpiceModel(ezxml_t Parent,
                              t_spice_model* spice_model);

static void ProcessSpiceModelPort(ezxml_t Node,
                                  t_spice_model_port* port);

static void ProcessSpiceModelPassGateLogic(ezxml_t Node,
                                    t_spice_model_pass_gate_logic* pass_gate_logic);

static void ProcessSpiceModelBuffer(ezxml_t Node,
                                    t_spice_model_buffer* buffer);

static void ProcessSpiceTransistorType(ezxml_t Parent,
                                       t_spice_transistor_type* spice_trans,
                                       enum e_spice_trans_type trans_type);

static void ProcessSpiceTechLibTransistors(ezxml_t Parent,
                                           t_spice_tech_lib* spice_tech_lib);

static 
void ProcessSpiceSRAMOrganization(INOUTP ezxml_t Node, 
                                  OUTP t_sram_inf_orgz* cur_sram_inf_orgz,
                                  boolean required);

/************ Subroutines***********/
static void ProcessSpiceMeasParams(ezxml_t Parent,
                                   t_spice_meas_params* meas_params) {
  ezxml_t Node, Cur;
  /* Check */
  if (meas_params == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])meas_params is NULL!\n", __FILE__, __LINE__);
    exit(1);
  }
  /* Number of simulation clock cycles */
  if (0 == strcmp("auto", FindProperty(Parent, "sim_num_clock_cycle", FALSE))) {
    meas_params->sim_num_clock_cycle = -1;
    meas_params->auto_select_sim_num_clk_cycle = TRUE;
  } else {
    meas_params->sim_num_clock_cycle = GetIntProperty(Parent, "sim_num_clock_cycle", FALSE, -1);
    meas_params->auto_select_sim_num_clk_cycle = FALSE;
  }
  ezxml_set_attr(Parent, "sim_num_clock_cycle", NULL);
  /* Accuracy type: either frac or abs, set frac by default*/
  if (NULL == FindProperty(Parent, "accuracy_type", FALSE)) {
    meas_params->accuracy_type = SPICE_ABS;
    meas_params->accuracy = 1e-13;
  } else {
    if (0 == strcmp(FindProperty(Parent, "accuracy_type", TRUE), "abs")) {
      meas_params->accuracy_type = SPICE_ABS;
    } else if (0 == strcmp(FindProperty(Parent, "accuracy_type", TRUE), "frac")) {
      meas_params->accuracy_type = SPICE_FRAC;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "(File:%s, [LINE%d])Invalid accuracy_type at ARCH_XML[LINE%d]! Expect [frac|abs].\n",
                 __FILE__, __LINE__, Parent->line);
    }
    meas_params->accuracy = GetFloatProperty(Parent, "accuracy", TRUE, 1e-13);
  }
  ezxml_set_attr(Parent, "accuracy", NULL);
  ezxml_set_attr(Parent, "accuracy_type", NULL);

  /* Process slew parameters*/
  Node = FindElement(Parent, "slew", FALSE);
  if (Node) {
    /* Find rise*/
    Cur = FindElement(Node, "rise", FALSE);
    if (Cur) { 
      /* Search for properties*/
      /* Rise */
      meas_params->slew_upper_thres_pct_rise = GetFloatProperty(Cur, "upper_thres_pct", TRUE, 0.9);
      ezxml_set_attr(Cur, "upper_thres_pct", NULL);
      meas_params->slew_lower_thres_pct_rise = GetFloatProperty(Cur, "lower_thres_pct", TRUE, 0.1);
      ezxml_set_attr(Cur, "lower_thres_pct", NULL);
      /* Free */
      FreeNode(Cur);
    }
    /* Find fall*/
    Cur = FindElement(Node, "fall", FALSE);
    if (Cur) { 
      meas_params->slew_upper_thres_pct_fall = GetFloatProperty(Cur, "upper_thres_pct", TRUE, 0.1);
      ezxml_set_attr(Cur, "upper_thres_pct", NULL);
      meas_params->slew_lower_thres_pct_fall = GetFloatProperty(Cur, "lower_thres_pct", TRUE, 0.9);
      ezxml_set_attr(Cur, "lower_thres_pct", NULL);
      /* Free */
      FreeNode(Cur);
    } 
    FreeNode(Node);
  }

  /* Process delay parameters*/
  Node = FindElement(Parent, "delay", FALSE);
  if (Node) {
    /* Find rise*/
    Cur = FindElement(Node, "rise", FALSE);
    if (Cur) { 
      /* Search for properties*/
      /* Rise */
      meas_params->input_thres_pct_rise = GetFloatProperty(Cur, "input_thres_pct", TRUE, 0.5);
      ezxml_set_attr(Cur, "input_thres_pct", NULL);
      meas_params->output_thres_pct_rise = GetFloatProperty(Cur, "output_thres_pct", TRUE, 0.5);
      ezxml_set_attr(Cur, "output_thres_pct", NULL);
      /* Free */
      FreeNode(Cur);
    }
    /* Find fall*/
    Cur = FindElement(Node, "fall", FALSE);
    if (Cur) { 
      meas_params->input_thres_pct_fall = GetFloatProperty(Cur, "input_thres_pct", TRUE, 0.5);
      ezxml_set_attr(Cur, "input_thres_pct", NULL);
      meas_params->output_thres_pct_fall = GetFloatProperty(Cur, "output_thres_pct", TRUE, 0.5);
      ezxml_set_attr(Cur, "output_thres_pct", NULL);
      /* Free */
      FreeNode(Cur);
    } 
    FreeNode(Node);
  }

  return;
}

static void ProcessSpiceStimulateParamsRiseFall(ezxml_t Parent,
                                                float* rise_param,
                                                float* fall_param,
                                                enum e_spice_accuracy_type* rise_type,
                                                enum e_spice_accuracy_type* fall_type) {
  ezxml_t Node;

  /* Check */
  assert(NULL != rise_param);
  assert(NULL != fall_param);
  assert(NULL != rise_type);
  assert(NULL != fall_type);

  /* initial to 0 */
  (*rise_param) = 0.;
  (*fall_param) = 0.;
  (*rise_type) = SPICE_FRAC;
  (*fall_type) = SPICE_FRAC;
  /* Rise parameters */
  Node = FindElement(Parent, "rise", TRUE);
  if (Node) {
    if (0 == strcmp("frac", FindProperty(Node, "slew_type", TRUE))) {
      (*rise_type) = SPICE_FRAC;
    } else if (0 == strcmp("abs", FindProperty(Node, "slew_type", TRUE))) {
      (*rise_type) = SPICE_ABS;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "Property(%s) should be defined in [LINE%d]!\n",
                 "slew_type" ,Node->line);
      exit(1);
    }
    (*rise_param) = GetFloatProperty(Node, "slew_time", TRUE, 0.05);
    ezxml_set_attr(Node, "slew_time", NULL);
    ezxml_set_attr(Node, "slew_type", NULL);
    /* Free */ 
    FreeNode(Node);
  }

  /* Fall parameters */
  Node = FindElement(Parent, "fall", TRUE);
  if (Node) {
    if (0 == strcmp("frac", FindProperty(Node, "slew_type", TRUE))) {
      (*fall_type) = SPICE_FRAC;
    } else if (0 == strcmp("abs", FindProperty(Node, "slew_type", TRUE))) {
      (*fall_type) = SPICE_ABS;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR, "Property(%s) should be defined in [LINE%d]!\n",
                 "slew_type" ,Node->line);
      exit(1);
    }
    (*fall_param) = GetFloatProperty(Node, "slew_time", TRUE, 0.05);
    ezxml_set_attr(Node, "slew_time", NULL);
    ezxml_set_attr(Node, "slew_type", NULL);
    /* Free */ 
    FreeNode(Node);
  }

  return;
}

static void ProcessSpiceStimulateParams(ezxml_t Parent,
                                        t_spice_stimulate_params* stimulate_params) {
  ezxml_t Node;

  /* Check */
  if (stimulate_params == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])stimulate_params is NULL!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Find Clock */
  Node = FindElement(Parent, "clock", FALSE);
  if (Node) {
    /* op_freq, sim_slack, prog_freq */
    stimulate_params->op_clock_freq = OPEN;
    /* op_freq (operation clock frequency) must be defined, either as a number or as "auto" */
    if (0 == strcmp("auto", FindProperty(Node, "op_freq", TRUE))) {
      /* We need a sim_slack */
    } else {
      stimulate_params->op_clock_freq = GetFloatProperty(Node, "op_freq", TRUE, OPEN);
      /* We do not need a sim_slack */
    } 
    ezxml_set_attr(Node, "op_freq", NULL);
    /* Read sim_slack */
    stimulate_params->sim_clock_freq_slack = GetFloatProperty(Node, "sim_slack", FALSE, 0.2);
    ezxml_set_attr(Node, "sim_slack", NULL);
    /* Read prog_freq (programming clock frequency): mandatory! */
    stimulate_params->prog_clock_freq = GetFloatProperty(Node, "prog_freq", TRUE, OPEN);
    /* For rising/falling slew */
    ProcessSpiceStimulateParamsRiseFall(Node, &(stimulate_params->clock_slew_rise_time), &(stimulate_params->clock_slew_fall_time), &(stimulate_params->clock_slew_rise_type), &(stimulate_params->clock_slew_fall_type));
    /* Free */
    FreeNode(Node);
  }

  /* Find input */
  Node = FindElement(Parent, "input", FALSE);
  if (Node) {
    /* Free */
    /* For rising/falling slew */
    ProcessSpiceStimulateParamsRiseFall(Node, &(stimulate_params->input_slew_rise_time), &(stimulate_params->input_slew_fall_time), &(stimulate_params->input_slew_rise_type), &(stimulate_params->input_slew_fall_type));
    FreeNode(Node);
  }

  return;
}

static void ProcessSpiceMCVariationParams(ezxml_t Parent,
                                          t_spice_mc_variation_params* variation_params) {
  /* Check */
  if (variation_params == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])variation_params is NULL!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  variation_params->abs_variation = GetFloatProperty(Parent, "abs_variation", TRUE, 0);
  ezxml_set_attr(Parent, "abs_variation", NULL);

  variation_params->num_sigma = GetIntProperty(Parent, "num_sigma", TRUE, 1);
  ezxml_set_attr(Parent, "num_sigma", NULL);
  
  return;
}

static void ProcessSpiceMonteCarloParams(ezxml_t Parent, 
                                         t_spice_mc_params* mc_params) {
  ezxml_t Node;

  /* Check */
  if (mc_params == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])mc_params is NULL!\n", 
               __FILE__, __LINE__);
    exit(1);
  }

  mc_params->mc_sim = FALSE;
  if (0 == strcmp("on", FindProperty(Parent, "mc_sim", TRUE))) {
    mc_params->mc_sim = TRUE;
  }
  ezxml_set_attr(Parent, "mc_sim", NULL);
  
  mc_params->num_mc_points = GetIntProperty(Parent, "num_mc_points", mc_params->mc_sim, 1);
  ezxml_set_attr(Parent, "num_mc_points", NULL);

  /* Process CMOS variations */
  mc_params->cmos_variation.variation_on = FALSE;
  if (NULL == FindProperty(Parent, "cmos_variation", FALSE)) {
    mc_params->cmos_variation.variation_on = FALSE;
  } else if (0 == strcmp("on", FindProperty(Parent, "cmos_variation", FALSE))) {
    mc_params->cmos_variation.variation_on = TRUE;
  } 
  Node = FindElement(Parent, "cmos", mc_params->cmos_variation.variation_on);
  if (Node) {
    ProcessSpiceMCVariationParams(Node, &(mc_params->cmos_variation));
    FreeNode(Node);
  }
  ezxml_set_attr(Parent, "cmos_variation", NULL);
  

  mc_params->rram_variation.variation_on = FALSE;
  if (NULL == FindProperty(Parent, "rram_variation", FALSE)) {
    mc_params->rram_variation.variation_on = FALSE;
  } else if (0 == strcmp("on", FindProperty(Parent, "rram_variation", FALSE))) {
    mc_params->rram_variation.variation_on = TRUE;
  }
  Node = FindElement(Parent, "rram", mc_params->rram_variation.variation_on);
  if (Node) {
    ProcessSpiceMCVariationParams(Node, &(mc_params->rram_variation));
    FreeNode(Node);
  }
  ezxml_set_attr(Parent, "rram_variation", NULL);

  mc_params->wire_variation.variation_on = FALSE;
  if (NULL == FindProperty(Parent, "wire_variation", FALSE)) {
    mc_params->wire_variation.variation_on = FALSE;
  } else if (0 == strcmp("on", FindProperty(Parent, "wire_variation", FALSE))) {
    mc_params->wire_variation.variation_on = TRUE;
  }
  Node = FindElement(Parent, "wire", mc_params->wire_variation.variation_on);
  if (Node) {
    ProcessSpiceMCVariationParams(Node, &(mc_params->wire_variation));
    FreeNode(Node);
  }
  ezxml_set_attr(Parent, "wire_variation", NULL);


  return;
}

static void ProcessSpiceParams(ezxml_t Parent,
                               t_spice_params* spice_params) {
  ezxml_t Node;

  InitSpiceParams(spice_params);

  /* Check */
  if (spice_params == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"(File: %s,[LINE%d])spice_params is NULL!\n", __FILE__, __LINE__);
    exit(1);
  }

  /* Options*/
  Node = FindElement(Parent, "options", FALSE);
  if (Node) {
    spice_params->sim_temp = GetIntProperty(Node, "sim_temp", TRUE, 25);
    ezxml_set_attr(Node, "sim_temp", NULL);
    /* OPTION post */
    if (0 == strcmp("on", FindProperty(Node, "post", TRUE))) {
      spice_params->post = TRUE;  
    } else if (0 == strcmp("off", FindProperty(Node, "post", TRUE))) {
      spice_params->post = FALSE;  
    } else {
      vpr_printf(TIO_MESSAGE_INFO, "Invalid value for post(Arch_file, LINE[%d])!\n", Node->line);
      exit(1); 
    }
    ezxml_set_attr(Node, "post", NULL);
    /* OPTION captab */
    if (0 == strcmp("on", FindProperty(Node, "captab", TRUE))) {
      spice_params->captab = TRUE;  
    } else if (0 == strcmp("off", FindProperty(Node, "captab", TRUE))) {
      spice_params->captab = FALSE;  
    } else {
      vpr_printf(TIO_MESSAGE_INFO, "Invalid value for captab(Arch_file, LINE[%d])!\n", Node->line);
      exit(1); 
    }
    ezxml_set_attr(Node, "captab", NULL);
    /* OPTION fast */
    if (0 == strcmp("on", FindProperty(Node, "fast", TRUE))) {
      spice_params->fast = TRUE;  
    } else if (0 == strcmp("off", FindProperty(Node, "fast", TRUE))) {
      spice_params->fast = FALSE;  
    } else {
      vpr_printf(TIO_MESSAGE_INFO, "Invalid value for fast(Arch_file, LINE[%d])!\n", Node->line);
      exit(1); 
    }
    ezxml_set_attr(Node, "fast", NULL);
    /* Free*/
    FreeNode(Node);
  }

  /* Process Monte Carlo Settings */
  Node = FindElement(Parent, "monte_carlo", FALSE);
  if (Node) {
    ProcessSpiceMonteCarloParams(Node, &(spice_params->mc_params));
    FreeNode(Node);
  }

  /* Process measure parameters*/
  Node = FindElement(Parent, "measure", FALSE);
  if (Node) {
    ProcessSpiceMeasParams(Node, &(spice_params->meas_params));
    FreeNode(Node);
  }

  /* Process stimulate parameters*/
  Node = FindElement(Parent, "stimulate", FALSE);
  if (Node) {
    ProcessSpiceStimulateParams(Node, &(spice_params->stimulate_params));
    FreeNode(Node);
  }
  
  return;
}

static void ProcessSpiceTransistorType(ezxml_t Parent,
                                       t_spice_transistor_type* spice_trans,
                                       enum e_spice_trans_type trans_type) {
  if (spice_trans == NULL) {
    vpr_printf(TIO_MESSAGE_ERROR,"ProcessSpiceTransistorType: spice_trans is NULL!\n");
    exit(1);
  }
  spice_trans->type = trans_type;
  spice_trans->model_name = my_strdup(FindProperty(Parent, "model_name", TRUE));
  ezxml_set_attr(Parent, "model_name", NULL);
  spice_trans->chan_length = GetFloatProperty(Parent,"chan_length",TRUE,0);
  ezxml_set_attr(Parent, "chan_length", NULL);
  spice_trans->min_width = GetFloatProperty(Parent,"min_width",TRUE,0);
  ezxml_set_attr(Parent, "min_width", NULL);
} 

static void ProcessSpiceModelBuffer(ezxml_t Node,
                                    t_spice_model_buffer* buffer) {
  boolean read_buf_info = FALSE;
  boolean read_spice_model = FALSE;
  char* Prop = NULL;
  /* Be smart to find all the details */
  /* Find "exist"*/
  Prop = my_strdup(FindProperty(Node, "exist", FALSE));
  if (NULL == Prop) {
    buffer->exist = 0;
  } else if (0 == strcmp(Prop,"on")) {
    buffer->exist = 1;
  } else if (0 == strcmp(Prop,"off")) {
    buffer->exist = 0;
  }
  ezxml_set_attr(Node, "exist", NULL);

  /* If buffer existed, we need to further find the spice_model_name */
  if (1 == buffer->exist) {
    read_buf_info = FALSE;
    read_spice_model = TRUE;
  } else if (0 == strcmp("design_technology", Node->name)) {
  /* Only under the design technology Node, this contains buffer information */
    read_buf_info = TRUE;
    read_spice_model = FALSE;
  }

  buffer->spice_model_name = my_strdup(FindProperty(Node, "spice_model_name", read_spice_model));
  ezxml_set_attr(Node, "spice_model_name", NULL); 

  /*Find Type*/
  Prop = my_strdup(FindProperty(Node, "topology", read_buf_info));
  if (NULL != Prop) {
    if (0 == strcmp(Prop,"inverter")) {
      buffer->type = SPICE_MODEL_BUF_INV;
    } else if (0 == strcmp(Prop,"buffer")) {
      buffer->type = SPICE_MODEL_BUF_BUF;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for type in input_buffer. Should be [inverter|buffer].\n",
                 Node->line);
      exit(1);
    }
  }
  ezxml_set_attr(Node, "topology", NULL);

  /*Find Tapered*/
  Prop = my_strdup(FindProperty(Node, "tapered", read_buf_info));
  if (NULL != Prop) {
    if (0 == strcmp(Prop,"on")) {
      buffer->tapered_buf = 1;
      /* Try to dig more properites ...*/
     buffer->tap_buf_level = GetIntProperty(Node, "tap_buf_level", TRUE, 1);
     buffer->f_per_stage = GetIntProperty(Node, "f_per_stage", FALSE, 4);
      ezxml_set_attr(Node, "tap_buf_level", NULL);
      ezxml_set_attr(Node, "f_per_stage", NULL);
    } else if (0 == strcmp(FindProperty(Node,"tapered",TRUE),"off")) {
      buffer->tapered_buf = 0;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for tapered buffer. Should be [on|off].\n",
                 Node->line);
      exit(1);
    } 
  }
  ezxml_set_attr(Node, "tapered", NULL);

  /* Find size*/
  buffer->size = GetFloatProperty(Node, "size", read_buf_info, 0.);
  ezxml_set_attr(Node, "size", NULL);

  return;
}

static void ProcessSpiceModelPassGateLogic(ezxml_t Node,
                                           t_spice_model_pass_gate_logic* pass_gate_logic) {
  if (0 == strcmp(FindProperty(Node,"topology",TRUE),"transmission_gate")) {
    pass_gate_logic->type = SPICE_MODEL_PASS_GATE_TRANSMISSION;
  } else if (0 == strcmp(FindProperty(Node,"topology",TRUE),"pass_transistor")) {
    pass_gate_logic->type = SPICE_MODEL_PASS_GATE_TRANSISTOR;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid topology of pass_gate_logic. Should be [transmission_gate|pass_transistor].\n",
              Node->line);
    exit(1);
  } 
  ezxml_set_attr(Node, "topology", NULL);

  pass_gate_logic->nmos_size = GetFloatProperty(Node,"nmos_size",TRUE,0);
  ezxml_set_attr(Node, "nmos_size", NULL);

  /* If the type of pass_gate_logic is pass transistor, 
   * the pmos_size is the size of level-restorer at the output node
   */
  pass_gate_logic->pmos_size = GetFloatProperty(Node,"pmos_size",TRUE,0);
  ezxml_set_attr(Node, "pmos_size", NULL);

  return;
}

static void ProcessSpiceModelPort(ezxml_t Node,
                                  t_spice_model_port* port) {
  if (0 == strcmp(FindProperty(Node,"type",TRUE),"input")) {
    port->type = SPICE_MODEL_PORT_INPUT;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"output")) {
    port->type = SPICE_MODEL_PORT_OUTPUT;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"clock")) {
    port->type = SPICE_MODEL_PORT_CLOCK;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"sram")) {
    port->type = SPICE_MODEL_PORT_SRAM;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"bl")) {
    port->type = SPICE_MODEL_PORT_BL;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"wl")) {
    port->type = SPICE_MODEL_PORT_WL;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"blb")) {
    port->type = SPICE_MODEL_PORT_BLB;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"wlb")) {
    port->type = SPICE_MODEL_PORT_WLB;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"inout")) {
    port->type = SPICE_MODEL_PORT_INOUT;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of port. Should be [input|output|clock|sram|bl|wl].\n",
               Node->line);
    exit(1);
  } 
  ezxml_set_attr(Node, "type", NULL);
  /* Assign prefix and size*/
  port->prefix = my_strdup(FindProperty(Node,"prefix",TRUE));
  ezxml_set_attr(Node, "prefix", NULL);
  port->size = GetIntProperty(Node,"size",TRUE,1);
  ezxml_set_attr(Node, "size", NULL);
  
  /* See if this is a mode selector.
   * Currently, we only allow a SRAM port to be a mode selector */
  if (SPICE_MODEL_PORT_SRAM == port->type) {
    port->mode_select = GetBooleanProperty(Node, "mode_select", FALSE, FALSE);
    ezxml_set_attr(Node, "mode_select", NULL);
  }

  /* Assign a default value for a port, which is useful in customizing the SRAM bit of idle LUTs */
  port->default_val = GetIntProperty(Node, "default_val", FALSE, 0);
  ezxml_set_attr(Node, "default_val", NULL);

  /* See if this is a global signal 
   * We assume that global signals are shared by all the SPICE Model/blocks.
   * We need to check if other SPICE model has the same port name
   */
  port->is_global = GetBooleanProperty(Node, "is_global", FALSE, FALSE);
  ezxml_set_attr(Node, "is_global", NULL);

  /* Check if this port is a set or reset port */ 
  port->is_reset = GetBooleanProperty(Node, "is_reset", FALSE, FALSE);
  ezxml_set_attr(Node, "is_reset", NULL);
  port->is_set = GetBooleanProperty(Node, "is_set", FALSE, FALSE);
  ezxml_set_attr(Node, "is_set", NULL);
  port->is_prog = GetBooleanProperty(Node, "is_prog", FALSE, FALSE);
  ezxml_set_attr(Node, "is_prog", NULL);

  /* Check if this port is a config_done port */ 
  port->is_config_enable = GetBooleanProperty(Node, "is_config_enable", FALSE, FALSE);
  ezxml_set_attr(Node, "is_config_enable", NULL);

  /* Check if this port is linked to another spice_model*/
  port->spice_model_name = my_strdup(FindProperty(Node,"spice_model_name",FALSE));
  ezxml_set_attr(Node, "spice_model_name", NULL);

  /* For BL/WL, BLB/WLB ports, we need to get the spice_model for inverters */
  if ((SPICE_MODEL_PORT_BL == port->type)
    ||(SPICE_MODEL_PORT_WL == port->type) 
    ||(SPICE_MODEL_PORT_BLB == port->type) 
    ||(SPICE_MODEL_PORT_WLB == port->type)) {
    port->inv_spice_model_name = my_strdup(FindProperty(Node, "inv_spice_model_name", FALSE));
    ezxml_set_attr(Node, "inv_spice_model_name", NULL);
  }
 
  return;
}

static void ProcessSpiceModelWireParam(ezxml_t Parent,
                                       t_spice_model_wire_param* wire_param) {
  if (0 == strcmp("pie",FindProperty(Parent,"model_type",TRUE))) {
    wire_param->type = WIRE_MODEL_PIE;
  } else if (0 == strcmp("t",FindProperty(Parent,"model_type",TRUE))) {
    wire_param->type = WIRE_MODEL_T;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of wire model(%s). Should be [pie|t].\n",
               Parent->line,FindProperty(Parent,"model_type",TRUE));
    exit(1);
  }

  ezxml_set_attr(Parent,"model_type",NULL);

  wire_param->res_val = GetFloatProperty(Parent,"res_val",TRUE,0);
  wire_param->cap_val = GetFloatProperty(Parent,"cap_val",TRUE,0);
  wire_param->level = GetIntProperty(Parent,"level",TRUE,0);

  ezxml_set_attr(Parent,"res_val",NULL);
  ezxml_set_attr(Parent,"cap_val",NULL);
  ezxml_set_attr(Parent,"level",NULL);
  
  return;
}

static void ProcessSpiceModel(ezxml_t Parent,
                              t_spice_model* spice_model) {
  ezxml_t Node, Cur;
  int iport;

  /* Basic Information*/
  if (0 == strcmp(FindProperty(Parent,"type",TRUE),"mux")) {
    spice_model->type = SPICE_MODEL_MUX;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"chan_wire")) {
    spice_model->type = SPICE_MODEL_CHAN_WIRE;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"wire")) {
    spice_model->type = SPICE_MODEL_WIRE;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"hard_logic")) {
    spice_model->type = SPICE_MODEL_HARDLOGIC;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"lut")) {
    spice_model->type = SPICE_MODEL_LUT;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"ff")) {
    spice_model->type = SPICE_MODEL_FF;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"sram")) {
    spice_model->type = SPICE_MODEL_SRAM;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"hard_logic")) {
    spice_model->type = SPICE_MODEL_HARDLOGIC;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"sff")) {
    spice_model->type = SPICE_MODEL_SCFF;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"iopad")) {
    spice_model->type = SPICE_MODEL_IOPAD;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"wire_vdd")) {
    spice_model->type = SPICE_MODEL_VDD;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"wire_gnd")) {
    spice_model->type = SPICE_MODEL_GND;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"inv_buf")) {
    spice_model->type = SPICE_MODEL_INVBUF;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"pass_gate")) {
    spice_model->type = SPICE_MODEL_PASSGATE;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of spice model(%s). Should be [mux|lut|ff|io|sram|hard_logic|sff].\n",
               Parent->line, FindProperty(Parent, "type", TRUE));
    exit(1);
  }
  ezxml_set_attr(Parent, "type", NULL);
  spice_model->name = my_strdup(FindProperty(Parent,"name",TRUE));
  ezxml_set_attr(Parent, "name", NULL);
  spice_model->prefix = my_strdup(FindProperty(Parent,"prefix",TRUE));
  ezxml_set_attr(Parent, "prefix", NULL);
 
  /* Find a spice_netlist path if we can*/
  spice_model->model_netlist = my_strdup(FindProperty(Parent,"spice_netlist",FALSE));
  if (spice_model->model_netlist) {
    ezxml_set_attr(Parent, "spice_netlist", NULL);
  }

  /* Find a verilog_netlist path if we can*/
  spice_model->verilog_netlist = my_strdup(FindProperty(Parent,"verilog_netlist",FALSE));
  if (spice_model->verilog_netlist) {
    ezxml_set_attr(Parent, "verilog_netlist", NULL);
  }

  /* Find the is_default if we can*/
  spice_model->is_default = GetIntProperty(Parent,"is_default",FALSE,0);
  ezxml_set_attr(Parent, "default", NULL);

  /* Find a verilog_netlist path if we can*/
  spice_model->dump_structural_verilog = GetBooleanProperty(Parent,"dump_structural_verilog", FALSE, FALSE);
  ezxml_set_attr(Parent, "dump_structural_verilog", NULL);
 
  /* Check the design technology settings*/
  Node = ezxml_child(Parent, "design_technology");
  if (Node) {
    /* Specify if this spice_model is power gated or not*/
    spice_model->design_tech_info.power_gated = GetBooleanProperty(Node,"power_gated", FALSE, FALSE);
	ezxml_set_attr(Node, "power_gated", NULL);
    /* More options*/
    if (0 == strcmp(FindProperty(Node,"type",TRUE),"cmos")) {
      spice_model->design_tech = SPICE_MODEL_DESIGN_CMOS;
      /* If this spice model is an inverter, buffer or pass_gate,
       * we need to read more settings
       */
      spice_model->design_tech_info.buffer_info = NULL;
      spice_model->design_tech_info.pass_gate_info = NULL;
      switch (spice_model->type) {
      case SPICE_MODEL_INVBUF:
         spice_model->design_tech_info.buffer_info = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
         ProcessSpiceModelBuffer(Node,spice_model->design_tech_info.buffer_info);
         break;
      case SPICE_MODEL_PASSGATE:
         spice_model->design_tech_info.pass_gate_info = (t_spice_model_pass_gate_logic*)my_malloc(sizeof(t_spice_model_pass_gate_logic));
         ProcessSpiceModelPassGateLogic(Node,spice_model->design_tech_info.pass_gate_info);
         break;
      default:
         break;
      }
    } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"rram")) {
      spice_model->design_tech = SPICE_MODEL_DESIGN_RRAM;
      /* RRAM tech. We need more properties*/
      spice_model->design_tech_info.ron = GetFloatProperty(Node,"ron",TRUE,0);
	  ezxml_set_attr(Node, "ron", NULL);
      spice_model->design_tech_info.roff = GetFloatProperty(Node,"roff",TRUE,0);
	  ezxml_set_attr(Node, "roff", NULL);
      spice_model->design_tech_info.wprog_set_nmos = GetFloatProperty(Node,"wprog_set_nmos",TRUE,0);
	  ezxml_set_attr(Node, "wprog_set_nmos", NULL);
      spice_model->design_tech_info.wprog_set_pmos = GetFloatProperty(Node,"wprog_set_pmos",TRUE,0);
	  ezxml_set_attr(Node, "wprog_set_nmos", NULL);
      spice_model->design_tech_info.wprog_reset_nmos = GetFloatProperty(Node,"wprog_reset_nmos",TRUE,0);
	  ezxml_set_attr(Node, "wprog_reset_nmos", NULL);
      spice_model->design_tech_info.wprog_reset_pmos = GetFloatProperty(Node,"wprog_reset_pmos",TRUE,0);
	  ezxml_set_attr(Node, "wprog_reset_pmos", NULL);
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for design_technology in spice model(%s). Should be [cmos|rram].\n",
                 Node->line,spice_model->name);
      exit(1);
    }
	ezxml_set_attr(Node, "type", NULL);
    /* Read in the structure if defined */
    if (SPICE_MODEL_MUX == spice_model->type) {
      if (0 == strcmp(FindProperty(Node,"structure",TRUE),"tree")) {
        spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_TREE;
      } else if (0 == strcmp(FindProperty(Node,"structure",TRUE),"one-level")) {
        spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
      } else if (0 == strcmp(FindProperty(Node,"structure",TRUE),"multi-level")) {
        spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_MULTILEVEL;
      /* New Structure: crossbar 
      } else if (0 == strcmp(FindProperty(Node,"structure",TRUE),"crossbar")) {
        spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_CROSSBAR;
      */
      } else {
         /* Set default to RRAM MUX */
         if (SPICE_MODEL_DESIGN_RRAM == spice_model->design_tech) {
           spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
         } else {
         /* Set default to SRAM MUX */
           spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_TREE;
        }
      }
    } else {
      /* Default: tree */
      spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_TREE;
    }
	ezxml_set_attr(Node, "structure", NULL);
    if (SPICE_MODEL_STRUCTURE_MULTILEVEL == spice_model->design_tech_info.structure) {
      spice_model->design_tech_info.mux_num_level = GetIntProperty(Node,"num_level",TRUE,1);
      /* For num_level == 1, auto convert to one-level structure */
      if (1 == spice_model->design_tech_info.mux_num_level) {
        spice_model->design_tech_info.structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
        vpr_printf(TIO_MESSAGE_INFO,"[LINE%d] Automatically convert structure of spice model(%s) to one-level.\n",
                   Node->line, spice_model->name);
      }
    } else if (SPICE_MODEL_STRUCTURE_ONELEVEL == spice_model->design_tech_info.structure) {
    /* Set mux_num_level for other structure: one-level and tree */
      spice_model->design_tech_info.mux_num_level = 1;
    }
	ezxml_set_attr(Node, "num_level", NULL);
    /* Specify if should use the advanced 4T1R MUX design */
    spice_model->design_tech_info.advanced_rram_design = GetBooleanProperty(Node,"advanced_rram_design", FALSE, FALSE);
    ezxml_set_attr(Node, "advanced_rram_design", NULL);
    FreeNode(Node);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] design_technology is expected in spice_model(%s).\n",
              Node->line,spice_model->name);
    exit(1);
  }

  /* LUT input_buffers */
  Node = ezxml_child(Parent, "lut_input_buffer");
  spice_model->lut_input_buffer = NULL;
  if (Node) {
    /* Malloc the lut_input_buffer */
    spice_model->lut_input_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
    ProcessSpiceModelBuffer(Node,spice_model->lut_input_buffer);
    FreeNode(Node);
  } else if (SPICE_MODEL_LUT == spice_model->type) {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] lut_input_buffer is expected in spice_model(%s).\n",
               Parent->line, spice_model->name);
    exit(1);
  } 
  /* Input Buffers*/
  Node = ezxml_child(Parent, "input_buffer");
  spice_model->input_buffer = NULL;
  if (Node) {
    /*Alloc*/
    spice_model->input_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
    ProcessSpiceModelBuffer(Node,spice_model->input_buffer);
    FreeNode(Node);
  } else if (SPICE_MODEL_INVBUF != spice_model->type) {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] input_buffer is expected in spice_model(%s).\n",
               Parent->line,spice_model->name);
    exit(1);
  } 
  /* Output Buffers*/
  Node = ezxml_child(Parent, "output_buffer");
  spice_model->output_buffer = NULL;
  if (Node) {
    spice_model->output_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
    ProcessSpiceModelBuffer(Node,spice_model->output_buffer);
    FreeNode(Node);
  } else if (SPICE_MODEL_INVBUF != spice_model->type) {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] output_buffer is expected in spice_model(%s).\n",
               Parent->line,spice_model->name);
    exit(1);
  }

  /* Pass_gate_logic*/
  Node = ezxml_child(Parent, "pass_gate_logic");
  spice_model->pass_gate_logic = NULL;
  if (Node) {
    spice_model->pass_gate_logic = (t_spice_model_pass_gate_logic*)my_malloc(sizeof(t_spice_model_pass_gate_logic));
    /* Find spice_model_name */
    spice_model->pass_gate_logic->spice_model_name = my_strdup(FindProperty(Node, "spice_model_name", TRUE));
	ezxml_set_attr(Node, "spice_model_name", NULL);
    FreeNode(Node);
  } else if ((SPICE_MODEL_MUX == spice_model->type)
            ||(SPICE_MODEL_LUT == spice_model->type)) {
    /* We have some exceptions: VDD, GND, dff, sram and hard_logic*/
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] pass_gate_logic is expected in spice_model(%s).\n",
               Node->line,spice_model->name);
    exit(1);
  }

  /* Find All the ports*/
  spice_model->num_port = CountChildren(Parent, "port", 1);
  /*Alloc*/
  spice_model->ports = (t_spice_model_port*)my_malloc(spice_model->num_port*sizeof(t_spice_model_port));
  /* Assign each found spice model*/
  for (iport = 0; iport < spice_model->num_port; iport++) {
    Cur = FindFirstElement(Parent, "port", TRUE);
    ProcessSpiceModelPort(Cur,&(spice_model->ports[iport]));
    FreeNode(Cur); 
  }

  /* Read in wire parameters */
  if ((SPICE_MODEL_CHAN_WIRE == spice_model->type)||(SPICE_MODEL_WIRE == spice_model->type)) {
    spice_model->wire_param = (t_spice_model_wire_param*)my_malloc(sizeof(t_spice_model_wire_param));
    Node = ezxml_child(Parent, "wire_param");
    if (Node) {
      ProcessSpiceModelWireParam(Node,spice_model->wire_param);
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] wire_param is expected in spice_model(%s).\n",
               Node->line,spice_model->name);
      exit(1);
    }
    FreeNode(Node);
  }

  /* Initialize the counter*/
  spice_model->cnt = 0;

  return;
}

/* Read XML under the Node of Organization */
static 
void ProcessSpiceSRAMOrganization(INOUTP ezxml_t Node, 
                                  OUTP t_sram_inf_orgz* cur_sram_inf_orgz,
                                  boolean required) {
  const char *Prop;

  if (NULL == Node) {
    return;
  } 

  cur_sram_inf_orgz->spice_model_name = my_strdup(FindProperty(Node, "spice_model_name", required));
  cur_sram_inf_orgz->spice_model = NULL;
  ezxml_set_attr(Node, "spice_model_name", NULL);

  /* read organization type*/
  Prop = FindProperty(Node, "organization", required);
  if (NULL == Prop) {
    cur_sram_inf_orgz->type = SPICE_SRAM_STANDALONE; /* Default */
  } else if (0 == strcmp("scan-chain", Prop)) {
    cur_sram_inf_orgz->type = SPICE_SRAM_SCAN_CHAIN;
  } else if (0 == strcmp("memory_bank", Prop)) {
    cur_sram_inf_orgz->type = SPICE_SRAM_MEMORY_BANK;
   } else if (0 == strcmp("standalone", Prop)) {
    cur_sram_inf_orgz->type = SPICE_SRAM_STANDALONE;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,
			"[LINE %d] Unknown property %s for SRAM organization\n",
 			 Node->line, FindProperty(Node, "organization", required));
    exit(1);
  }
  ezxml_set_attr(Node, "organization", NULL);

  return;
}

/* Read XML under the Node of SRAM */
void ProcessSpiceSRAM(INOUTP ezxml_t Node, OUTP struct s_arch* arch) {
  ezxml_t Cur;

  /* Process area */
  if (NULL == Node) {
    return;
  } 

  arch->sram_inf.area = GetFloatProperty(Node, "area", FALSE, 6);

  /* Read the SPICE sram organization details */
  Cur = FindElement(Node, "spice", arch->read_xml_spice);
  if (NULL != Cur) {
    /* Malloc */
    arch->sram_inf.spice_sram_inf_orgz = (t_sram_inf_orgz*)my_malloc(sizeof(t_sram_inf_orgz));
    ProcessSpiceSRAMOrganization(Cur, arch->sram_inf.spice_sram_inf_orgz, arch->read_xml_spice);
    FreeNode(Cur);
  } else {
    arch->sram_inf.spice_sram_inf_orgz = NULL;
  }

  /* Read the SPICE sram organization details */
  Cur = FindElement(Node, "verilog", arch->read_xml_spice);
  if (NULL != Cur) {
    /* Malloc */
    arch->sram_inf.verilog_sram_inf_orgz = (t_sram_inf_orgz*)my_malloc(sizeof(t_sram_inf_orgz));
    ProcessSpiceSRAMOrganization(Cur, arch->sram_inf.verilog_sram_inf_orgz, arch->read_xml_spice);
    FreeNode(Cur);
  } else {
    arch->sram_inf.verilog_sram_inf_orgz = NULL;
  }

  return;
}

/* Check Codes: We should check if we need to define the I/O transistors */
static void check_tech_lib(t_spice_tech_lib tech_lib, 
                           int num_spice_model,
                           t_spice_model* spice_models) {
  int i;
  int rram_mux_found = 0; 
  int io_nmos_found = 0;
  int io_pmos_found = 0;

  for (i = 0; i < num_spice_model; i++) {
    if ((SPICE_MODEL_MUX == spice_models[i].type)
       &&(SPICE_MODEL_DESIGN_RRAM == spice_models[i].design_tech)) {
      rram_mux_found = 1;
      break;
    }
  }
  /* RRAM MUX is not defined, no need to check the tech library*/
  if (0 == rram_mux_found) {
    return;
  }

  /* RRAM MUX is defined, check the tech library by searching for io_nmos and io_pmos */
  for (i = 0; i < tech_lib.num_transistor_type; i++) {
    if (SPICE_TRANS_IO_NMOS == tech_lib.transistor_types[i].type) {
      io_nmos_found = 1;
    }     
    if (SPICE_TRANS_IO_PMOS == tech_lib.transistor_types[i].type) {
      io_pmos_found = 1;
    }     
  }
  if ((0 == io_nmos_found)||(0 == io_pmos_found)) {
    vpr_printf(TIO_MESSAGE_ERROR, "(File:%s,[LINE%d]) I/O transistors are not defined for RRAM MUX!\nCheck your tech_lib!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return;
}

/* Check Codes: We should check the spice models */
static void check_spice_models(int num_spice_model,
                               t_spice_model* spice_models) {
  int i,j;
  int has_sram = 0;
  int has_io = 0;
  int has_mux = 0;
  int has_in_port = 0;
  int has_out_port = 0;
  int has_clock_port = 0;
  int has_sram_port = 0;

  for (i = 0; i < num_spice_model; i++) {
    /* Check whether spice models share the same name or prefix*/
    for (j = 0; j < num_spice_model; j++) {
      if (i == j) {
        continue;
      }
      if (0 == strcmp(spice_models[i].prefix,spice_models[j].prefix)) {
        vpr_printf(TIO_MESSAGE_ERROR,"Spice model(%s) and (%s) share the same prefix(%s), which is invalid!\n",
                  spice_models[i].name,spice_models[j].name,spice_models[i].prefix);
        exit(1);
      }
      if (0 == strcmp(spice_models[i].name,spice_models[j].name)) {
        vpr_printf(TIO_MESSAGE_ERROR,"Spice model(%s) and (%s) share the same name(%s), which is invalid!\n",
                  spice_models[i].name,spice_models[j].name,spice_models[i].name);
        exit(1);
      }
    }
    /* Check io has been defined and has input and output ports*/
    if (SPICE_MODEL_IOPAD == spice_models[i].type) {
      has_io = 1;
      has_in_port = 0;
      has_out_port = 0;
      for (j = 0; j < spice_models[i].num_port; j++) {
        if (SPICE_MODEL_PORT_INPUT == spice_models[i].ports[j].type) {
          has_in_port = 1;
        } else if (SPICE_MODEL_PORT_OUTPUT == spice_models[i].ports[j].type) {
          has_out_port = 1;
        }
      }
      /* Check if we have two ports*/
      if ((0 == has_in_port)||(0 == has_out_port)) {
        vpr_printf(TIO_MESSAGE_ERROR,"IO Spice model(%s) does not have input|output port\n",spice_models[i].name);
        exit(1);
      }
    }
    /* Check mux has been defined and has input and output ports*/
    if (SPICE_MODEL_MUX == spice_models[i].type) {
      has_mux = 1;
      has_in_port = 0;
      has_out_port = 0;
      has_sram_port = 0;
      for (j = 0; j < spice_models[i].num_port; j++) {
        if (SPICE_MODEL_PORT_INPUT == spice_models[i].ports[j].type) {
          has_in_port = 1;
        } else if (SPICE_MODEL_PORT_OUTPUT == spice_models[i].ports[j].type) {
          has_out_port = 1;
        } else if (SPICE_MODEL_PORT_SRAM == spice_models[i].ports[j].type) {
          has_sram_port = 1;
        }
      }
      /* Check if we have two ports*/
      if ((0 == has_in_port)||(0 == has_out_port)||(0 == has_sram_port)) {
        vpr_printf(TIO_MESSAGE_ERROR,"MUX Spice model(%s) does not have input|output|sram port\n",spice_models[i].name);
        exit(1);
      }
      /* Check the I/O transistors are defined when RRAM MUX is selected */
      if (SPICE_MODEL_DESIGN_RRAM == spice_models[i].design_tech) {
        if (!(0. < spice_models[i].design_tech_info.wprog_set_nmos)) {
          vpr_printf(TIO_MESSAGE_ERROR, "wprog_set_nmos(%g) should be >0 for a RRAM MUX SPICE model (%s)!\n",
                     spice_models[i].design_tech_info.wprog_set_nmos, spice_models[i].name);
          exit(1);
        }
        if (!(0. < spice_models[i].design_tech_info.wprog_set_pmos)) {
          vpr_printf(TIO_MESSAGE_ERROR, "wprog_set_pmos(%g) should be >0 for a RRAM MUX SPICE model (%s)!\n",
                     spice_models[i].design_tech_info.wprog_set_pmos, spice_models[i].name);
          exit(1);
        }
        if (!(0. < spice_models[i].design_tech_info.wprog_reset_nmos)) {
          vpr_printf(TIO_MESSAGE_ERROR, "wprog_reset_nmos(%g) should be >0 for a RRAM MUX SPICE model (%s)!\n",
                     spice_models[i].design_tech_info.wprog_reset_nmos, spice_models[i].name);
          exit(1);
        }
        if (!(0. < spice_models[i].design_tech_info.wprog_reset_pmos)) {
          vpr_printf(TIO_MESSAGE_ERROR, "wprog_reset_pmos(%g) should be >0 for a RRAM MUX SPICE model (%s)!\n",
                     spice_models[i].design_tech_info.wprog_reset_pmos, spice_models[i].name);
          exit(1);
        }
      } 
    }
    /* Check sram has been defined and has input and output ports*/
    if (SPICE_MODEL_SRAM == spice_models[i].type) {
      has_sram = 1;
      has_in_port = 0;
      has_out_port = 0;
      for (j = 0; j < spice_models[i].num_port; j++) {
        if (SPICE_MODEL_PORT_INPUT == spice_models[i].ports[j].type) {
          has_in_port = 1;
        } else if (SPICE_MODEL_PORT_OUTPUT == spice_models[i].ports[j].type) {
          has_out_port = 1;
        }
      }
      /* Check if we have two ports*/
      if ((0 == has_in_port)||(0 == has_out_port)) {
        vpr_printf(TIO_MESSAGE_ERROR,"SRAM Spice model(%s) does not have input|output port\n",spice_models[i].name);
        exit(1);
      }
    }
    /* Check dff has input and output, clock ports*/
    if (SPICE_MODEL_FF == spice_models[i].type) {
      has_clock_port = 0;
      has_in_port = 0;
      has_out_port = 0;
      for (j = 0; j < spice_models[i].num_port; j++) {
        if (SPICE_MODEL_PORT_INPUT == spice_models[i].ports[j].type) {
          has_in_port = 1;
        } else if (SPICE_MODEL_PORT_OUTPUT == spice_models[i].ports[j].type) {
          has_out_port = 1;
        } else if (SPICE_MODEL_PORT_CLOCK == spice_models[i].ports[j].type) {
          has_clock_port = 1;
        }
      }
      /* Check if we have two ports*/
      if ((0 == has_in_port)||(0 == has_out_port)||(0 == has_clock_port)) {
        vpr_printf(TIO_MESSAGE_ERROR,"FF Spice model(%s) does not have input|output|clock port\n",spice_models[i].name);
        exit(1);
      }
    }
    /* Check lut has input and output, clock ports*/
    if (SPICE_MODEL_LUT == spice_models[i].type) {
      has_sram_port = 0;
      has_in_port = 0;
      has_out_port = 0;
      for (j = 0; j < spice_models[i].num_port; j++) {
        if (SPICE_MODEL_PORT_INPUT == spice_models[i].ports[j].type) {
          has_in_port = 1;
        } else if (SPICE_MODEL_PORT_OUTPUT == spice_models[i].ports[j].type) {
          has_out_port = 1;
        } else if (SPICE_MODEL_PORT_SRAM == spice_models[i].ports[j].type) {
          has_sram_port = 1;
        }
      }
      /* Check if we have two ports*/
      if ((0 == has_in_port)||(0 == has_out_port)||(0 == has_sram_port)) {
        vpr_printf(TIO_MESSAGE_ERROR,"LUT Spice model(%s) does not have input|output|sram port\n",spice_models[i].name);
        exit(1);
      }
    }
  }
  
  if ((0 == has_io)||(0 == has_sram)||(0 == has_mux)) {
    vpr_printf(TIO_MESSAGE_ERROR,"At least 1 io,sram,mux spice model should be defined!\n");
    exit(1);
  }

  return;
}

static void ProcessSpiceTechLibTransistors(ezxml_t Parent,
                                           t_spice_tech_lib* spice_tech_lib) {
  ezxml_t Node, Cur;  
  int itrans;

  /* TODO: move to a function for this */
  /* Check the tech_lib*/
  Node = FindElement(Parent, "tech_lib", TRUE);
  if (Node) {
    /* Check the type first*/
    if (0 == strcmp("academia",FindProperty(Node, "lib_type", TRUE))) {
      spice_tech_lib->type = SPICE_LIB_ACADEMIA;
    } else if (0 == strcmp("industry",FindProperty(Node, "lib_type", TRUE))) {
      spice_tech_lib->type = SPICE_LIB_INDUSTRY;
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] tech_lib type should be [industry|academia].\n",
                 Node->line);
      exit(1);
    }
	ezxml_set_attr(Node, "lib_type", NULL);

    if (SPICE_LIB_INDUSTRY == spice_tech_lib->type) {
      spice_tech_lib->transistor_type = my_strdup(FindProperty(Node, "transistor_type", TRUE));
    } 
	ezxml_set_attr(Node, "transistor_type", NULL);

    spice_tech_lib->path = my_strdup(FindProperty(Node, "lib_path", TRUE));
	ezxml_set_attr(Node, "lib_path", NULL);
    /* Norminal VDD for standard transistors*/
    spice_tech_lib->nominal_vdd = GetFloatProperty(Node, "nominal_vdd", TRUE, 0.);
    ezxml_set_attr(Node, "nominal_vdd", NULL);

    /* Norminal VDD for IO transistors : by default, give the value of nominal vdd */
    spice_tech_lib->io_vdd = GetFloatProperty(Node, "io_vdd", FALSE, spice_tech_lib->nominal_vdd);
    ezxml_set_attr(Node, "io_vdd", NULL);

    /* Current Node search ends*/
    FreeNode(Node);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] tech_lib does not exist in spice_settings.\n",
               Node->line);
    exit(1);
  }
  /* Check the transistors*/
  Node = FindElement(Parent, "transistors", TRUE);
  if (Node) {
    spice_tech_lib->pn_ratio = GetFloatProperty(Node,"pn_ratio", TRUE, 0);
    spice_tech_lib->model_ref = my_strdup(FindProperty(Node,"model_ref", TRUE));
    ezxml_set_attr(Node, "pn_ratio", NULL);
    ezxml_set_attr(Node, "model_ref", NULL);
    /* Fill transistor_type number*/
    spice_tech_lib->num_transistor_type = 0;
    spice_tech_lib->num_transistor_type += CountChildren(Node, "nmos", 1);
    assert(1 == spice_tech_lib->num_transistor_type);
    spice_tech_lib->num_transistor_type += CountChildren(Node, "pmos", 1);
    assert(2 == spice_tech_lib->num_transistor_type);
    spice_tech_lib->num_transistor_type += CountChildren(Node, "io_nmos", 0);
    spice_tech_lib->num_transistor_type += CountChildren(Node, "io_pmos", 0);
    assert((2 == spice_tech_lib->num_transistor_type)||(4 == spice_tech_lib->num_transistor_type));
    /*Alloc*/
    spice_tech_lib->transistor_types = (t_spice_transistor_type*)my_malloc(spice_tech_lib->num_transistor_type*sizeof(t_spice_transistor_type));
    /* Fill Standard NMOS */
    itrans = 0;
    Cur = FindFirstElement(Node, "nmos", TRUE);
    ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_NMOS);
    FreeNode(Cur);
    itrans++;
    /* Fill Standard PMOS */
    Cur = FindFirstElement(Node, "pmos", TRUE);
    ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_PMOS);
    FreeNode(Cur);
    itrans++;
    /* Standard NMOS and PMOS are mandatory */
    assert(2 == itrans);
    /* Fill IO NMOS */
    Cur = FindFirstElement(Node, "io_nmos", FALSE);
    if (NULL != Cur) {
      ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_IO_NMOS);
      FreeNode(Cur);
      itrans++;
      vpr_printf(TIO_MESSAGE_INFO, "Read I/O NMOS into tech. lib. successfully.\n");
    }
    /* Fill IO PMOS */
    Cur = FindFirstElement(Node, "io_pmos", FALSE);
    if (NULL != Cur) {
      ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_IO_PMOS);
      FreeNode(Cur);
      itrans++;
      vpr_printf(TIO_MESSAGE_INFO, "Read I/O PMOS into tech. lib. successfully.\n");
    }
    assert((2 == itrans)||(4 == itrans));
    /* Finish parsing this node*/
    FreeNode(Node);
  }

  return;
}

/* Process the SPICE Settings*/
void ProcessSpiceSettings(ezxml_t Parent,
                          t_spice* spice) {
  ezxml_t Node, Cur;
  int imodel;

  InitSpice(spice);

  /* Check the parameters*/
  Node = FindElement(Parent, "parameters", FALSE);
  if (Node) {
    ProcessSpiceParams(Node, &(spice->spice_params));
    FreeNode(Node);
  }

  /* Technology Library and Transistors */
  ProcessSpiceTechLibTransistors(Parent, &(spice->tech_lib));
  
  /* module spice models*/
  Node = FindElement(Parent, "module_spice_models", FALSE);
  if (Node) {
    spice->num_spice_model = CountChildren(Node, "spice_model", 1);
    /*Alloc*/
    spice->spice_models = (t_spice_model*)my_malloc(spice->num_spice_model*sizeof(t_spice_model));
    /* Assign each found spice model*/
    for (imodel = 0; imodel < spice->num_spice_model; imodel++) {
      Cur = FindFirstElement(Node, "spice_model", TRUE);
      ProcessSpiceModel(Cur, &(spice->spice_models[imodel]));
      FreeNode(Cur); 
    }
    assert(imodel == spice->num_spice_model);
    FreeNode(Node);
  }
 
  /* Check codes*/
  check_tech_lib(spice->tech_lib, spice->num_spice_model, spice->spice_models);
  check_spice_models(spice->num_spice_model,spice->spice_models);

  return;
}


