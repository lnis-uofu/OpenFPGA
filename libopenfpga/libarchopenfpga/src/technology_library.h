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
  TECH_LIB_ACADEMIA,
  NUM_TECH_LIB_TYPES
};
/* Strings correspond to each technology library type */
constexpr std::array<const char*, NUM_TECH_LIB_TYPES> TECH_LIB_TYPE_STRING = {{"industry", "academia"}};

/********************************************************************
 * Types of transistor groups which may be defined in a technology library
 * We categorize the transistors in terms of their usage in FPGA architecture
 * 1. NMOS transistor 
 * 2. PMOS transistor 
 *******************************************************************/
enum e_tech_lib_trans_type {
  TECH_LIB_TRANS_NMOS, 
  TECH_LIB_TRANS_PMOS, 
};
/* Strings correspond to transistor type type */
constexpr std::array<const char*, NUM_TECH_LIB_TYPES> TECH_LIB_TRANS_TYPE_STRING = {{"industry", "academia"}};

/********************************************************************
 * Types of transistors which may be defined in a technology library
 * 1. NMOS transistor 
 * 2. PMOS transistor 
 *******************************************************************/
enum e_tech_lib_trans_type {
  TECH_LIB_TRANS_NMOS, 
  TECH_LIB_TRANS_PMOS, 
};
/* Strings correspond to transistor type type */
constexpr std::array<const char*, NUM_TECH_LIB_TYPES> TECH_LIB_TRANS_TYPE_STRING = {{"industry", "academia"}};

/********************************************************************
 * Process corners supported 
 *******************************************************************/
enum e_process_corner {
 BEST_CORNER,
 TYPICAL_CORNER,
 WORST_CORNER
};

#endif
