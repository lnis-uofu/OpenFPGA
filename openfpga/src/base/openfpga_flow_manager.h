#ifndef FLOW_MANAGER_H
#define FLOW_MANAGER_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * FlowManager aims to resolve the dependency between OpenFPGA functional
 * code blocks
 * It can provide flags for downstream modules about if the data structures
 * they require have already been constructed
 *
 *******************************************************************/
class FlowManager {
  public: /* Public constructor */
    FlowManager();
  public: /* Public accessors */
    bool compress_routing() const;
  public: /* Public mutators */
    void set_compress_routing(const bool& enabled);
  private: /* Internal Data */
    bool compress_routing_;
};

} /* End namespace openfpga*/

#endif
