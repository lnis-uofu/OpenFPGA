#ifndef TECHNOLOGY_LIBRARY_H
#define TECHNOLOGY_LIBRARY_H

/********************************************************************
 * This file include the declaration of technology library
 *******************************************************************/

/********************************************************************
 * Types for technology library attributes
 * Industrial library: the .lib file which define technology  
 *                     This is ubiquitous in commercial vendors
 *                     For industry library, we allow users to define
 *                     process corners.
 * Academia library: the .pm file which define technology 
 *                   This is mainly used in PTM-like technology library 
 * PTM is the Predictive Technology Model provided by the Arizona
 * State University (ASU). Available at ptm.asu.edu
 *******************************************************************/
enum e_tech_lib_type {
  TECH_LIB_INDUSTRY,
  TECH_LIB_ACADEMIA
};

/********************************************************************
 * Types of transistors which may be defined in a technology library
 * We categorize the transistors in terms of their usage in FPGA architecture
 * 1. NMOS transistor used in datapath logic
 * 2. PMOS transistor used in datapath logic
 * 3. NMOS transistor used in the I/O blocks
 * 3. PMOS transistor used in the I/O blocks
 *******************************************************************/
enum e_tech_lib_trans_type {
  TECH_LIB_TRANS_NMOS, 
  TECH_LIB_TRANS_PMOS, 
  TECH_LIB_TRANS_IO_NMOS, 
  TECH_LIB_TRANS_IO_PMOS
};

/********************************************************************
 * Process corners supported 
 *******************************************************************/
enum e_process_corner {
 BEST_CORNER,
 TYPICAL_CORNER,
 WORST_CORNER
};

#endif
