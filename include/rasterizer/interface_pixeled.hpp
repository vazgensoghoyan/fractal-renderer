#pragma once // rasterizer/interface_pixeled.hpp

#include <concepts>
#include <type_traits>

namespace iheay::raster {

template <typename T>
concept HasRGB = requires(T t) {
    { t.r } -> std::convertible_to<int>;
    { t.g } -> std::convertible_to<int>;
    { t.b } -> std::convertible_to<int>;
};

template <HasRGB PixelType>
class IPixeled {
public:
    virtual int width() const = 0;
    virtual int height() const = 0;
    
    virtual PixelType get_pixel(int x, int y) const = 0;
    
    virtual void set_pixel(int x, int y, PixelType pixel) = 0;
    virtual bool try_set_pixel(int x, int y, PixelType pixel) = 0; // true if set
};

} // namespace iheay::raster
