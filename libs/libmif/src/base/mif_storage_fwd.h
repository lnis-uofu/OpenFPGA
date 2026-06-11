#ifndef MIF_STORAGE_FWD_H
#define MIF_STORAGE_FWD_H

#include "vtr_strong_id.h"

struct mif_segment_id_tag;
struct mif_memory_line_id_tag;

typedef vtr::StrongId<mif_segment_id_tag> MifSegmentId;
typedef vtr::StrongId<mif_memory_line_id_tag> MifMemoryLineId;

namespace openfpga {
class MifStorage;
}

#endif
