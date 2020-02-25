#ifndef VTR_LOGIC_H
#define VTR_LOGIC_H

#include <array>

namespace vtr {

enum class LogicValue {
    FALSE = 0,
    TRUE = 1,
    DONT_CARE = 2,
    UNKOWN = 3,
    NUM_LOGIC_VALUE_TYPES
};

constexpr std::array<const char*, size_t(LogicValue::NUM_LOGIC_VALUE_TYPES)> LOGIC_VALUE_STRING = {{"false", "true", "don't care", "unknown"}};

} // namespace vtr

#endif
