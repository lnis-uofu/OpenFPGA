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
    /* freq, sim_slack */
    stimulate_params->clock_freq = 0.;
    stimulate_params->clock_freq = GetFloatProperty(Node, "freq", FALSE, 0);
    ezxml_set_attr(Node, "freq", NULL);
    stimulate_params->sim_clock_freq_slack = GetFloatProperty(Node, "sim_slack", FALSE, 0.2);
    ezxml_set_attr(Node, "sim_slack", NULL);
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
  /* Find "exist"*/
  if (0 == strcmp(FindProperty(Node,"exist",TRUE),"on")) {
    buffer->exist = 1;
  } else if (0 == strcmp(FindProperty(Node,"exist",TRUE),"off")) {
    buffer->exist = 0;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for exist in input_buffer. Should be [on|off].\n",
               Node->line);
    exit(1);
  }
  ezxml_set_attr(Node, "exist", NULL);
  /*Find Type*/
  if (0 == strcmp(FindProperty(Node,"type",TRUE),"inverter")) {
    buffer->type = SPICE_MODEL_BUF_INV;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"buffer")) {
    buffer->type = SPICE_MODEL_BUF_BUF;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for type in input_buffer. Should be [inverter|buffer].\n",
               Node->line);
    exit(1);
  }

  /*Find Tapered*/
  if (0 == strcmp(FindProperty(Node,"tapered",TRUE),"on")) {
    buffer->tapered_buf = 1;
    /* Try to dig more properites ...*/
    buffer->tap_buf_level = GetIntProperty(Node, "tap_buf_level", TRUE, 1);
    buffer->f_per_stage = GetIntProperty(Node, "f_per_stage", FALSE, 4);
  } else if (0 == strcmp(FindProperty(Node,"tapered",TRUE),"off")) {
    buffer->tapered_buf = 0;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for tapered buffer. Should be [on|off].\n",
               Node->line);
    exit(1);
  } 
  ezxml_set_attr(Node, "type", NULL);
  ezxml_set_attr(Node, "tapered", NULL);
  ezxml_set_attr(Node, "tap_buf_level", NULL);
  ezxml_set_attr(Node, "f_per_stage", NULL);
  /* Find size*/
  buffer->size = GetFloatProperty(Node,"size",TRUE,0.);
  ezxml_set_attr(Node, "size", NULL);

  return;
}

static void ProcessSpiceModelPassGateLogic(ezxml_t Node,
                                           t_spice_model_pass_gate_logic* pass_gate_logic) {
  if (0 == strcmp(FindProperty(Node,"type",TRUE),"transmission_gate")) {
    pass_gate_logic->type = SPICE_MODEL_PASS_GATE_TRANSMISSION;
  } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"pass_transistor")) {
    pass_gate_logic->type = SPICE_MODEL_PASS_GATE_TRANSISTOR;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of pass_gate_logic. Should be [transmission_gate|pass_transistor].\n",
              Node->line);
    exit(1);
  } 
  ezxml_set_attr(Node, "type", NULL);
  pass_gate_logic->nmos_size = GetFloatProperty(Node,"nmos_size",TRUE,0);
  ezxml_set_attr(Node, "nmos_size", NULL);
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
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of port. Should be [input|output|clock|sram].\n",
               Node->line);
    exit(1);
  } 
  ezxml_set_attr(Node, "type", NULL);
  /* Assign prefix and size*/
  port->prefix = my_strdup(FindProperty(Node,"prefix",TRUE));
  ezxml_set_attr(Node, "prefix", NULL);
  port->size = GetIntProperty(Node,"size",TRUE,1);
  ezxml_set_attr(Node, "size", NULL);
 
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
  /*Alloc*/
  spice_model->input_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
  spice_model->output_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
  spice_model->pass_gate_logic = (t_spice_model_pass_gate_logic*)my_malloc(sizeof(t_spice_model_pass_gate_logic));
  /* Malloc the lut_input_buffer */
  spice_model->lut_input_buffer = (t_spice_model_buffer*)my_malloc(sizeof(t_spice_model_buffer));
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
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"inpad")) {
    spice_model->type = SPICE_MODEL_INPAD;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"outpad")) {
    spice_model->type = SPICE_MODEL_OUTPAD;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"sram")) {
    spice_model->type = SPICE_MODEL_SRAM;
  } else if (0 == strcmp(FindProperty(Parent,"type",TRUE),"hard_logic")) {
    spice_model->type = SPICE_MODEL_HARDLOGIC;
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid type of spice model(%s). Should be [mux|lut|ff|io|sram|hard_logic].\n",
               Parent->line, FindProperty(Parent, "type", TRUE));
    exit(1);
  }
  ezxml_set_attr(Parent, "type", NULL);
  spice_model->name = my_strdup(FindProperty(Parent,"name",TRUE));
  ezxml_set_attr(Parent, "name", NULL);
  spice_model->prefix = my_strdup(FindProperty(Parent,"prefix",TRUE));
  ezxml_set_attr(Parent, "prefix", NULL);
 
  /* Find a netlist path if we can*/
  spice_model->model_netlist = my_strdup(FindProperty(Parent,"netlist",FALSE));
  if (spice_model->model_netlist) {
    ezxml_set_attr(Parent, "netlist", NULL);
  }

  /* Find the is_default if we can*/
  spice_model->is_default = GetIntProperty(Parent,"is_default",FALSE,0);
  ezxml_set_attr(Parent, "default", NULL);
 
  /* Check the design settings*/
  Node = ezxml_child(Parent, "design_technology");
  if (Node) {
    if (0 == strcmp(FindProperty(Node,"type",TRUE),"cmos")) {
      spice_model->design_tech = SPICE_MODEL_DESIGN_CMOS;
    } else if (0 == strcmp(FindProperty(Node,"type",TRUE),"rram")) {
      spice_model->design_tech = SPICE_MODEL_DESIGN_RRAM;
      /* RRAM tech. We need more properties*/
      spice_model->ron = GetFloatProperty(Node,"ron",TRUE,0);
	  ezxml_set_attr(Node, "ron", NULL);
      spice_model->roff = GetFloatProperty(Node,"roff",TRUE,0);
	  ezxml_set_attr(Node, "roff", NULL);
      spice_model->prog_trans_size = GetFloatProperty(Node,"prog_transistor_size",TRUE,0);
	  ezxml_set_attr(Node, "prog_transistor_size", NULL);
    } else {
      vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] Invalid value for design_technology in spice model(%s). Should be [cmos|rram].\n",
                 Node->line,spice_model->name);
      exit(1);
    }
	ezxml_set_attr(Node, "type", NULL);
    /* Read in the structure if defined */
    if (SPICE_MODEL_MUX == spice_model->type) {
      if (0 == strcmp(FindProperty(Node,"structure",TRUE),"tree")) {
        spice_model->structure = SPICE_MODEL_STRUCTURE_TREE;
      } else if (0 == strcmp(FindProperty(Node,"structure",TRUE),"one-level")) {
        spice_model->structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
      } else if (0 == strcmp(FindProperty(Node,"structure",TRUE),"multi-level")) {
        spice_model->structure = SPICE_MODEL_STRUCTURE_MULTILEVEL;
      } else {
        /* Default: tree */
        spice_model->structure = SPICE_MODEL_STRUCTURE_TREE;
      }
    } else {
      /* Default: tree */
      spice_model->structure = SPICE_MODEL_STRUCTURE_TREE;
    }
	ezxml_set_attr(Node, "structure", NULL);
    if (SPICE_MODEL_STRUCTURE_MULTILEVEL == spice_model->structure) {
      spice_model->mux_num_level = GetIntProperty(Node,"num_level",TRUE,1);
      /* For num_level == 1, auto convert to one-level structure */
      if (1 == spice_model->mux_num_level) {
        spice_model->structure = SPICE_MODEL_STRUCTURE_ONELEVEL;
        vpr_printf(TIO_MESSAGE_INFO,"[LINE%d] Automatically convert structure of spice model(%s) to one-level.\n",
                   Node->line, spice_model->name);
      }
    }
	ezxml_set_attr(Node, "num_level", NULL);
    FreeNode(Node);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] design_technology is expected in spice_model(%s).\n",
              Node->line,spice_model->name);
    exit(1);
  }
  /* LUT input_buffers */
  Node = ezxml_child(Parent, "lut_input_buffer");
  if (Node) {
    ProcessSpiceModelBuffer(Node,spice_model->lut_input_buffer);
    FreeNode(Node);
  } else if (SPICE_MODEL_LUT == spice_model->type) {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] lut_input_buffer is expected in spice_model(%s).\n",
               Parent->line, spice_model->name);
    exit(1);
  } 
  /* Input Buffers*/
  Node = ezxml_child(Parent, "input_buffer");
  if (Node) {
    ProcessSpiceModelBuffer(Node,spice_model->input_buffer);
    FreeNode(Node);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] input_buffer is expected in spice_model(%s).\n",
               Parent->line,spice_model->name);
    exit(1);
  } 
  /* Output Buffers*/
  Node = ezxml_child(Parent, "output_buffer");
  if (Node) {
    ProcessSpiceModelBuffer(Node,spice_model->output_buffer);
    FreeNode(Node);
  } else {
    vpr_printf(TIO_MESSAGE_ERROR,"[LINE %d] output_buffer is expected in spice_model(%s).\n",
               Parent->line,spice_model->name);
    exit(1);
  }
  /* Pass_gate_logic*/
  Node = ezxml_child(Parent, "pass_gate_logic");
  if (Node) {
    ProcessSpiceModelPassGateLogic(Node,spice_model->pass_gate_logic);
    FreeNode(Node);
  } else if ((SPICE_MODEL_FF != spice_model->type)&&(SPICE_MODEL_SRAM != spice_model->type)&(SPICE_MODEL_HARDLOGIC != spice_model->type)&&(SPICE_MODEL_WIRE != spice_model->type)&&(SPICE_MODEL_CHAN_WIRE != spice_model->type)) { /* We have some exceptions: dff, sram and hard_logic*/
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
    /* TODO Check whether spice models share the same name or prefix*/
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
    if ((SPICE_MODEL_INPAD == spice_models[i].type)||(SPICE_MODEL_OUTPAD == spice_models[i].type)) {
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
    /* TODO Check mux has been defined and has input and output ports*/
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

    spice_tech_lib->nominal_vdd = GetFloatProperty(Node, "nominal_vdd", TRUE, 0.);
    ezxml_set_attr(Node, "nominal_vdd", NULL);

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
    /*Alloc*/
    spice_tech_lib->transistor_types = (t_spice_transistor_type*)my_malloc(spice_tech_lib->num_transistor_type*sizeof(t_spice_transistor_type));
    /* Fill nmos and pmos*/
    itrans = 0;
    Cur = FindFirstElement(Node, "nmos", TRUE);
    ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_NMOS);
    FreeNode(Cur);
    itrans++;
    Cur = FindFirstElement(Node, "pmos", TRUE);
    ProcessSpiceTransistorType(Cur,&(spice_tech_lib->transistor_types[itrans]),SPICE_TRANS_PMOS);
    FreeNode(Cur);
    itrans++;
    assert(2 == itrans);
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
  check_spice_models(spice->num_spice_model,spice->spice_models);

  return;
}
