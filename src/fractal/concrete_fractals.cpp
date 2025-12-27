#include "fractal/concrete_fractals.hpp"

using namespace iheay;
using namespace iheay::bmp;
using namespace iheay::math;

Bmp fractal::draw_mandelbrot(int width, int height) {
    bmp::Bmp img = bmp::Bmp::empty(width, height);

    fractal::Viewport view{
        Complex::Algebraic(-2.0, -1.5),
        Complex::Algebraic(1.0, 1.5)
    };

    fractal::render_complex_fractal(
        img, view,
        [](auto& z, auto& c) { return z*z + c; },
        [](auto&) { return Complex::Zero(); },
        [](auto& pixel) { return pixel; },
        {300, 2.0}
    );

    return img;
}

Bmp fractal::draw_julia(int width, int height) {
    bmp::Bmp img = bmp::Bmp::empty(width, height);

    fractal::Viewport view{
        Complex::Algebraic(-2.0, -1.5),
        Complex::Algebraic(1.0, 1.5)
    };

    Complex k = Complex::Algebraic(-0.8, 0.156);

    fractal::render_complex_fractal(
        img, view,
        [](auto& z, auto& c) { return z*z + c; },
        [](auto& pixel) { return pixel; },
        [&](auto&) { return k; },
        {300, 2.0}
    );

    return img;
}

Bmp fractal::draw_burning_ship(int width, int height) {
    bmp::Bmp img = bmp::Bmp::empty(width, height);

    fractal::Viewport view{
        Complex::Algebraic(-2.0, -1.5),
        Complex::Algebraic(1.0, 1.5)
    };

    fractal::render_complex_fractal(
        img, view,
        [](auto& z, auto& c) {
            return Complex::Algebraic(
                std::abs(z.get_real()),
                std::abs(z.get_imag())
            ).pow(2) + c;
        },
        [](auto&) { return Complex::Zero(); },
        [](auto& p) { return p; },
        {300, 2.0}
    );

    return img;
}
