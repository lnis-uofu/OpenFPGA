#ifndef FABRIC_SPICE_OPTIONS_H
#define FABRIC_SPICE_OPTIONS_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <string>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Options for Fabric Spice generator
 *******************************************************************/
class FabricSpiceOption {
  public: /* Public constructor */
    /* Set default options */
    FabricSpiceOption();
  public: /* Public accessors */
    std::string output_directory() const;
    bool explicit_port_mapping() const;
    bool compress_routing() const;
    bool verbose_output() const;
  public: /* Public mutators */
    void set_output_directory(const std::string& output_dir);
    void set_explicit_port_mapping(const bool& enabled);
    void set_compress_routing(const bool& enabled);
    void set_verbose_output(const bool& enabled);
  private: /* Internal Data */
    std::string output_directory_;
    bool explicit_port_mapping_;
    bool compress_routing_;
    bool verbose_output_;
};

} /* End namespace openfpga*/

#endif
