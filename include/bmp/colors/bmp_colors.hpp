#pragma once // bmp/colors/bmp_colors.hpp

#include "bmp/bmp_structs.hpp"

namespace iheay::bmp::colors {

inline constexpr iheay::bmp::BgrPixel WHITE {255, 255, 255};
inline constexpr iheay::bmp::BgrPixel BLACK {0, 0, 0};
inline constexpr iheay::bmp::BgrPixel RED   {0, 0, 255};
inline constexpr iheay::bmp::BgrPixel GREEN {0, 255, 0};
inline constexpr iheay::bmp::BgrPixel BLUE  {255, 0, 0};
inline constexpr iheay::bmp::BgrPixel YELLOW{0, 255, 255};
inline constexpr iheay::bmp::BgrPixel CYAN  {255, 255, 0};
inline constexpr iheay::bmp::BgrPixel MAGENTA {255, 0, 255};

} // namespace iheay::bmp::colors
