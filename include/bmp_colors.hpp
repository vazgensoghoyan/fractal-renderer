#pragma once

#include "bmp_structs.hpp"

namespace iheay::bmp::colors {
    inline constexpr iheay::bmp::Pixel WHITE {255, 255, 255};
    inline constexpr iheay::bmp::Pixel BLACK {0, 0, 0};
    inline constexpr iheay::bmp::Pixel RED   {0, 0, 255};
    inline constexpr iheay::bmp::Pixel GREEN {0, 255, 0};
    inline constexpr iheay::bmp::Pixel BLUE  {255, 0, 0};
    inline constexpr iheay::bmp::Pixel YELLOW{0, 255, 255};
    inline constexpr iheay::bmp::Pixel CYAN  {255, 255, 0};
    inline constexpr iheay::bmp::Pixel MAGENTA {255, 0, 255};
}
