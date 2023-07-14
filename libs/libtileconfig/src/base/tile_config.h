#ifndef TILE_CONFIG_H
#define TILE_CONFIG_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <array>
#include <string>

/* Begin namespace openfpga */
namespace openfpga {

/**
 * @brief tile configuration is a data structure to represent how programmable
 * blocks and routing blocks are grouped into tiles
 */
class TileConfig {
 public: /* Types */
  enum class e_style {
    TOP_LEFT = 0,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CUSTOM,
    NUM_TYPES
  };

 public: /* Constructors */
  TileConfig();

 public: /* Public accessors */
  /** @brief Get all the fpga style */
  e_style style() const;
  std::string style_to_string() const;

 public: /* Public mutators */
  /** @brief Create the one-on-one mapping between an port of fpga_top and
   * fpga_core. Return 0 for success, return 1 for fail */
  int set_style(const std::string& value);

 public: /* Public utility */
  /** @brief identify if the tile config is valid or not */
  bool is_valid() const;
  /** @brief Parse the style from string to valid type. Parser
   * error can be turned on */
  e_style str2style(const std::string& style_str,
                    const bool& verbose = false) const;
  /** @brief Output the string representing style */
  std::string style2str(const e_style& style,
                        const bool& verbose = false) const;
  /** @brief Validate the style */
  bool valid_style(const e_style& style) const;

 private: /* Internal utility */
  /* Generate a string include all the valid style
   * Useful for printing debugging messages */
  std::string style_all2str() const;

 private: /* Internal Data */
  e_style style_;
  /* Constants */
  std::array<const char*, size_t(e_style::NUM_TYPES)> STYLE_STRING_;
};

} /* End namespace openfpga*/

#endif
