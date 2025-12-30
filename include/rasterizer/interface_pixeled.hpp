#pragma once // rasterizer/interface_pixeled.hpp

#include <concepts>
#include <type_traits>

namespace iheay::raster {

template <typename T>
concept HasRGB = requires(T t) {
    requires std::integral<decltype(t.r)>;
    requires std::integral<decltype(t.g)>;
    requires std::integral<decltype(t.b)>;
};

template <HasRGB PixelType>
class IPixeled {
public:
    virtual ~IPixeled() = default;

    virtual int width() const = 0;
    virtual int height() const = 0;
    
    virtual const PixelType& get_pixel(int x, int y) const = 0;
    
    virtual void set_pixel(int x, int y, PixelType pixel) = 0;
    virtual bool try_set_pixel(int x, int y, PixelType pixel) = 0; // true if set
};

} // namespace iheay::raster
