/************************************************************************
 * A header file for CircuitLibrary class, including critical data declaration
 * Please include this file only for using any CircuitLibrary data structure
 * Refer to circuit_library.h for more details 
 ***********************************************************************/

/************************************************************************
 * Create strong id for Circuit Models/Ports to avoid illegal type casting 
 ***********************************************************************/
#include "vtr_strong_id.h"

struct circuit_model_id_tag;
struct circuit_port_id_tag;
struct circuit_edge_id_tag;

typedef vtr::StrongId<circuit_model_id_tag> CircuitModelId;
typedef vtr::StrongId<circuit_port_id_tag> CircuitPortId;
typedef vtr::StrongId<circuit_edge_id_tag> CircuitEdgeId;

/* Short declaration of class */
class CircuitLibrary;
