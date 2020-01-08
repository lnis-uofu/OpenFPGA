#include <string.h>
#include <assert.h>
#include "util.h"
#include "arch_types.h"
#include "ReadLine.h"
#include "ezxml.h"
#include "read_xml_arch_file.h"
#include "read_xml_util.h"

/* mrFPGA */
#include "read_xml_mrfpga.h"

/* mrFPGA: reshaped by Xifan TANG*/
void init_buffer_inf(t_buffer_inf* buffer_inf) {
  buffer_inf->C = 0.; 
  buffer_inf->R = 0.; 
  buffer_inf->Tdel = 0.; 

  return;
}

void init_memristor_inf(t_memristor_inf* memristor_inf) {
  memristor_inf->C = 0.;
  memristor_inf->R = 0.;
  memristor_inf->Tdel = 0.;

  return;
}

/* Initial the mrfpga architecture */
void init_arch_mrfpga(t_arch_mrfpga* arch_mrfpga) {
  /* Initial the booleans*/
  arch_mrfpga->is_isolation = FALSE;
  arch_mrfpga->is_stack = FALSE;
  arch_mrfpga->is_junction = FALSE;
  arch_mrfpga->is_wire_buffer = FALSE;
  arch_mrfpga->is_mrFPGA = FALSE;
  arch_mrfpga->is_accurate = FALSE;
  
  /*Initial some structs*/
  init_buffer_inf(&(arch_mrfpga->wire_buffer_inf));
  init_memristor_inf(&(arch_mrfpga->memristor_inf));

  /* Initial arch parameters*/
  arch_mrfpga->max_pins_per_side = 0;
  arch_mrfpga->main_best_buffer_list = NULL;
  arch_mrfpga->num_normal_switch = 0;
  arch_mrfpga->start_seg_switch = 0;

  /* RRAM and SRAM support*/
  arch_mrfpga->is_show_sram = FALSE;
  arch_mrfpga->is_show_pass_trans = FALSE;
  arch_mrfpga->tech_comp = CONV;
  arch_mrfpga->Rseg_global = 0.;
  arch_mrfpga->Cseg_global = 0.;
  arch_mrfpga->rram_pass_tran_value = 0.;

  arch_mrfpga->is_opin_cblock_defined = 0;

  return;
}

void
ProcessTechHack(INOUTP ezxml_t Node,
	      OUTP struct s_arch *arch) {
    const char *Prop;
    /* Xifan TANG: fill the struct mrfpga*/
    if ( Node ) {
        Prop = FindProperty(Node, "isolation", FALSE);
        if ( Prop ) {
            if ( strcmp(Prop,"no") == 0 ) {
                arch->arch_mrfpga.is_isolation = FALSE;
            }
            ezxml_set_attr(Node, "isolation", NULL);
        }
        Prop = FindProperty(Node, "stack", FALSE);
        if ( Prop ) {
            if ( strcmp(Prop,"no") == 0 ) {
                arch->arch_mrfpga.is_stack = FALSE;
            }
            ezxml_set_attr(Node, "stack", NULL);
        }
        Prop = FindProperty(Node, "junction", FALSE);
        if ( Prop ) {
            if ( strcmp(Prop,"no") == 0 ) {
                arch->arch_mrfpga.is_junction = FALSE;
            }
            ezxml_set_attr(Node, "junction", NULL);
        }
    }
  return;
}

void
ProcessWireBuffer(INOUTP ezxml_t Node,
	      OUTP struct s_arch *arch) {
    const char *Prop;
    if ( Node ) {
        /* is_wire_buffer*/
        arch->arch_mrfpga.is_wire_buffer = TRUE;

        /* Buffer resistance*/
        Prop = FindProperty(Node, "R", TRUE);
        arch->arch_mrfpga.wire_buffer_inf.R = atof(Prop);
        ezxml_set_attr(Node, "R", NULL);

        /* Buffer Cin*/
        Prop = FindProperty(Node, "Cin", TRUE);
        arch->arch_mrfpga.wire_buffer_inf.C = atof(Prop);
        ezxml_set_attr(Node, "Cin", NULL);

        /* TODO: (Xifan TANG) There should be a Cout for buffer ...*/
        ezxml_set_attr(Node, "Cout", NULL); 

        /* Buffer Tdel */
        Prop = FindProperty(Node, "Tdel", TRUE);
        arch->arch_mrfpga.wire_buffer_inf.Tdel = atof(Prop);
        ezxml_set_attr(Node, "Tdel", NULL);
    }
  return;
}

static void
ProcessTechComp(INOUTP ezxml_t Node,
	      OUTP struct s_arch *arch) {
    const char *Prop;
    /*
    Prop = FindProperty(Node, "paper", FALSE);
    if (Prop != NULL) {
        if (strcmp(Prop,"mono")==0) {
            arch->arch_mrfpga.tech_comp = MONO;
        } else if (strcmp(Prop,"sttram")==0) {
            arch->arch_mrfpga.tech_comp = STTRAM;
        } else if (strcmp(Prop,"pcram_xie")==0) {
            arch->arch_mrfpga.tech_comp = PCRAM_Xie;
        }else if (strcmp(Prop,"pcram_pierre")==0) {
            arch->arch_mrfpga.tech_comp = PCRAM_Pierre;
        } else if(strcmp(Prop,"nem")==0) {
            arch->arch_mrfpga.tech_comp = NEM;
        } else {
            printf("Unknown property %s for tech_comp value\n", Prop);
            exit(1);
        }
        ezxml_set_attr(Node, "paper", NULL);
    }
    */
    Prop = FindProperty(Node, "rram_pass_tran_value", FALSE);
    if(Prop != NULL) {
        arch->arch_mrfpga.rram_pass_tran_value = atof(Prop);
        ezxml_set_attr(Node, "rram_pass_tran_value", NULL);
    }
  return;
}

void
ProcessmrFPGA(INOUTP ezxml_t Node,
	      OUTP struct s_arch *arch) {
    const char *Prop;
    ezxml_t Cur;

    arch->arch_mrfpga.is_isolation = TRUE;
    arch->arch_mrfpga.is_junction = TRUE;
    arch->arch_mrfpga.is_stack = TRUE;

    /* Xifan TANG: Abolish hack */
    /*
    Cur = FindElement(Node, "hack", FALSE);
    ProcessTechHack(Cur,arch);
    */
    if (arch->arch_mrfpga.is_isolation && arch->arch_mrfpga.is_junction && arch->arch_mrfpga.is_stack) {
      arch->arch_mrfpga.is_mrFPGA = TRUE;
    } else {
      arch->arch_mrfpga.is_mrFPGA = FALSE;
    }

    if(arch->arch_mrfpga.is_junction) {
        /* Read Resistance */
        Prop = FindProperty(Node, "R", TRUE);
        arch->arch_mrfpga.memristor_inf.R = atof(Prop);
        ezxml_set_attr(Node, "R", NULL);
        /* Read Capacitance */
        Prop = FindProperty(Node, "C", TRUE);
        arch->arch_mrfpga.memristor_inf.C = atof(Prop);
        ezxml_set_attr(Node, "C", NULL);
        /* Read instrinsic delay*/
        Prop = FindProperty(Node, "Tdel", TRUE);
        arch->arch_mrfpga.memristor_inf.Tdel = atof(Prop);
        ezxml_set_attr(Node, "Tdel", NULL);
    }

    /* buffer */
    Cur = FindElement(Node, "buffer", FALSE);
    ProcessWireBuffer(Cur,arch);
    if(Cur) {
      FreeNode(Cur);
    }

    /* buffer */
    Cur = FindElement(Node, "cblock", FALSE);
    ProcessMrFPGATiming(Cur,arch);
    if(Cur) {
      FreeNode(Cur);
    }


  return;
}

void
ProcessTechnology(INOUTP ezxml_t Node,
	      OUTP struct s_arch *arch) {
    ezxml_t Cur;

    /* mrFPGA: Xifan TANG
     * I move the globals into a struct so that 
     * we can use static library link
     */
    init_arch_mrfpga(&(arch->arch_mrfpga));
    if ( Node ) {
        Cur = FindElement(Node, "mrFPGA", FALSE);
        if ( Cur ) {
            ProcessmrFPGA(Cur, arch);
            FreeNode(Cur);
        }
        Cur = FindElement(Node, "tech_comp", FALSE);
        if ( Cur ) {
            ProcessTechComp(Cur, arch);
            FreeNode(Cur);
        }
    }
  return;
}

/* Xifan TANG: mrFPGA has a child called timing, define the R_opin_cblock and T_opin_cblock */
void ProcessMrFPGATiming(INOUTP ezxml_t Cur, 
                         OUTP t_arch* arch) {
  const char *Prop = NULL;

  if(Cur != NULL) {
    /* set the defined flag*/
    arch->arch_mrfpga.is_opin_cblock_defined = 1;
 
    Prop = FindProperty(Cur, "R_opin_cblock", TRUE);
    arch->arch_mrfpga.R_opin_cblock = atof(Prop);
    ezxml_set_attr(Cur, "R_opin_cblock", NULL);

    Prop = FindProperty(Cur, "T_opin_cblock", TRUE);
    arch->arch_mrfpga.T_opin_cblock = atof(Prop);
    ezxml_set_attr(Cur, "T_opin_cblock", NULL);
  }

  return;
}

/* end */

