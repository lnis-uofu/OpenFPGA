#ifndef OPENFPGA_CONTEXT_H
#define OPENFPGA_CONTEXT_H

#include "openfpga_arch.h"

/********************************************************************
 * This file includes the declaration of the date structure 
 * OpenfpgaContext, which is used for data exchange between 
 * different modules in OpenFPGA shell environment 
 *
 * If a command of OpenFPGA needs to exchange data with other commands,
 * it must use this data structure to access/mutate.
 * In such case, you must add data structures to OpenfpgaContext
 *
 * Note:
 * Please respect to the following rules when using the OpenfpgaContext
 * 1. This data structure will be created only once in the main() function
 *    The data structure is design to be large and contain all the 
 *    data structure required by each module of OpenFPGA core engine.
 *    Do NOT create or duplicate in your own module!     
 * 2. Be clear in your mind if you want to access/mutate the data inside OpenfpgaContext
 *    Read-only data should be accessed by 
 *      const OpenfpgaContext& 
 *    Mutate should use reference
 *      OpenfpgaContext&
 * 3. Please keep the definition of OpenfpgaContext short
 *    Do put ONLY well-modularized data structure under this root.
 *******************************************************************/
class OpenfpgaContext {
  private:
    /* Data structure to store information from read_openfpga_arch library */
    openfpga::Arch arch_;
};

#endif
