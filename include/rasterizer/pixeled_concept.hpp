#pragma once // rasterizer/pixeled_concept.hpp

#include <concepts>
#include <type_traits>

namespace iheay::raster {

template<typename Image>
concept PixeledImage = requires(Image image, int x, int y) {
    typename Image::pixel_type;

    { image.width() } -> std::convertible_to<int>;
    { image.height() } -> std::convertible_to<int>;

    { image.set_pixel(x, y, typename Image::pixel_type{}) };
};

} // namespace iheay::raster
