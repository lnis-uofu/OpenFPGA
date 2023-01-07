#ifndef OPENFPGA_LUT_TRUTH_TABLE_FIXUP_H
#define OPENFPGA_LUT_TRUTH_TABLE_FIXUP_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void update_lut_tt_with_post_packing_results(
  const AtomContext& atom_ctx, const ClusteringContext& clustering_ctx,
  VprClusteringAnnotation& vpr_clustering_annotation, const bool& verbose);

} /* end namespace openfpga */

#endif
