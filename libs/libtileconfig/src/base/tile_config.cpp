/******************************************************************************
 * Memember functions for data structure TileConfig
 ******************************************************************************/
#include "tile_config.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

TileConfig::TileConfig() {
  style_ = TileConfig::e_style::NUM_TYPES; /* Deposit an invalid value */
  STYLE_STRING_ = {"top_left", "top_right", "bottom_left", "bottom_right",
                   "custom"};
}

/**************************************************
 * Public Accessors
 *************************************************/
TileConfig::e_style TileConfig::style() const { return style_; }

std::string TileConfig::style_to_string() const { return style2str(style_); }

bool TileConfig::is_valid() const {
  return style_ != TileConfig::e_style::NUM_TYPES;
}

int TileConfig::set_style(const std::string& value) {
  style_ = str2style(value);
  return valid_style(style_);
}

std::string TileConfig::style_all2str() const {
  std::string full_types = "[";
  for (int itype = size_t(TileConfig::e_style::TOP_LEFT);
       itype != size_t(TileConfig::e_style::NUM_TYPES); ++itype) {
    full_types += std::string(STYLE_STRING_[itype]) + std::string("|");
  }
  full_types.pop_back();
  full_types += "]";
  return full_types;
}

TileConfig::e_style TileConfig::str2style(const std::string& style_str,
                                          const bool& verbose) const {
  for (int itype = size_t(TileConfig::e_style::TOP_LEFT);
       itype != size_t(TileConfig::e_style::NUM_TYPES); ++itype) {
    if (style_str == std::string(STYLE_STRING_[itype])) {
      return static_cast<TileConfig::e_style>(itype);
    }
  }
  VTR_LOGV_ERROR(verbose, "Invalid style for tile configuration! Expect %s\n",
                 style_all2str().c_str());
  return TileConfig::e_style::NUM_TYPES;
}

std::string TileConfig::style2str(const TileConfig::e_style& style,
                                  const bool& verbose) const {
  if (!valid_style(style)) {
    VTR_LOGV_ERROR(verbose, "Invalid style for tile configuration! Expect %s\n",
                   style_all2str().c_str());
    return std::string();
  }
  return std::string(STYLE_STRING_[size_t(style)]);
}

bool TileConfig::valid_style(const TileConfig::e_style& style) const {
  return style != TileConfig::e_style::NUM_TYPES;
}

} /* end namespace openfpga */
