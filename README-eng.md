🇷🇺 **РУССКАЯ ВЕРСИЯ:** [README на русском](README.md)

# Fractal Renderer & Math Engine (C++)

**A modular C++ project for fractal generation, mathematical primitives, and rendering images in BMP format.**

Focuses on clean architecture, design patterns, generic programming (concepts, templates), numerical algorithms, and extensibility.

The project demonstrates:

* Modern C++ (C++20/23)
* Use of OOP principles
* Several design patterns (Builder, Factory Method, Adapter, etc.)
* Use of concepts
* Some math (complex numbers, quaternions, fractals)
* Working with binary image formats
* Modular architecture and tests

---

### Example Usage

The renderer is created via a **Fluent Builder** pattern. A specific `BgrColorizer` structure implements the required concept for fractal coloring. Some properties are set for the renderer (viewport, functions, etc.) and then we build it.

The example below illustrates rendering the Mandelbrot fractal:

```cpp
auto renderer = 
    FractalRendererBuilder<BgrColorizer>
        ::get_builder()
            .set_viewport_width(3)
            .set_viewport_center(-0.75)
            .set_iteration_func( [](auto& z, auto& c) { return z * z + c; } )
            .set_initial_func( [](auto&) { return Complex::Zero(); } )
            .set_param_func( [](auto& pixel) { return pixel; } )
            .build();

Bmp image = Bmp::empty(2000, 2000);
renderer.render(image);

io::save(image, "mandelbrot_set.bmp");
```

---

## 📁 Repository Structure

```
fractal-renderer/
│
├── external/           # Git submodules (third-party dependencies)
│   ├── googletest/
│   └── raylib/
│
├── include/
│   ├── adapters/
│   ├── bmp/            # BMP image structs and loading/saving
│   ├── fractal/        # Fractal generation algorithms
│   ├── math/           # Vec3, Complex, Quaternion, Ray
│   ├── rasterizer/     # Concept PixeledImage and rasterization algs for it
│   ├── ray_tracing/    # Ray tracing engine and related structures
│   └── utils/          # Shared helpers and utilities (currently only Logger)
│
├── src/                # Implementations
│   ├── ...
│   └── main.cpp
│
├── tests/
│   ├── bmp/            # Tests for BMP module
│   ├── math/           # Tests for math module
│   └── CMakeLists.txt
│
├── usage_examples/     # Small standalone examples demonstrating features
│   ├── CMakeLists.txt
│   ├── draw_julia_set.cpp
│   ├── draw_mandelbrot_set.cpp
│   └── render_animation_mandelbrot.cpp
│
├── CMakeLists.txt
└── README.md
```

---

## Main Features

### 🧠 Math Engine

Custom implementation of mathematical primitives (here is a brief overview of one):

* **`Complex`** — complex numbers
  Supports:

  * Algebraic and trigonometric forms (named constructors)
  * Arithmetic operations (+, −, *, /)
  * Modulus, normalized argument, conjugate
  * Power
  * n‑th roots
  * Comparison with EPS tolerance
  * Formatted output via `std::format`
  * Extensive use of `constexpr` and `[[nodiscard]]`

* **`Quaternion`**

* **`Vec3`**

* **`Ray`**

This engine can be used independently of fractals — as a lightweight library for numerical and graphics tasks.

---

### 🖼 Image Handling (BMP)

A minimalist module for working with **24-bit BMPs**:

#### Features

* Loading and saving BMP without third-party libraries
* Supports only:
  * 24 bpp
  * Uncompressed (BI_RGB)
* Explicit handling of BMP header structures
* Control over structure alignment via `#pragma pack`

#### Bmp class

Generic representation of a raster image:

```cpp
Bmp image = Bmp::empty(1920, 1080);
image.set_pixel(x, y, {b, g, r});

Bmp image2 = Bmp::empty(4000, 4000, colors::WHITE);
BgrPixel pixel = image2.get_pixel(12, 122);
image2.try_set_pixel(-10, 1, {b, g, r}); // no exception, returns true if set
```

#### namespace bmp::io

Loading and saving logic is separated from the class itself to respect the `Single Responsibility Principle`:

```cpp
Bmp image = io::load("some/interesting/path");
// transformations
io::save(image, "some/unique/path");
```

---

### 🌌 Fractal Renderer

A **generic fractal rendering engine** that is independent of image format or fractal formula — both are supplied externally via template parameters:

| Component       | Responsibility                          |
| --------------- | --------------------------------------- |
| `IterationFunc` | Iteration formula (e.g., `z = z² + c`)  |
| `InitialFunc`   | Initial value `z_0`                     |
| `ParamFunc`     | Fractal parameter (e.g., `c` for Julia) |
| `Colorizer`     | Maps iteration count → pixel color      |

Enables building:

* Mandelbrot sets
* Julia sets
* Custom fractals with arbitrary formulas

#### 🎨 Coloring System (Colorizer Concept)

C++ concepts enforce the requirements for colorizers:

```cpp
template <typename Colorizer>
concept ColorizerConcept = requires(Colorizer c, double mu, int max_iter) {
    typename Colorizer::pixel_type;
    { c(mu, max_iter) } -> std::same_as<typename Colorizer::pixel_type>;
};
```

The renderer knows nothing about the image format—only that a `pixel_type` exists (here, BGR for BMP).
This allows **any image type** satisfying the `PixeledImage` concept to be used.

---

### 🎞 Fractal Animation

A **keyframe system** is implemented:

```cpp
struct FractalKeyframe {
    Viewport viewport;
    FractalConfig config;
    math::Complex julia_c;
};
```

Supports interpolation between states:

```cpp
FractalKeyframe k = interpolate(a, b, t);
```

This is the foundation for zoom animations and fractal morphing.

---

### 🧵 Generic Rendering via Concepts

Renderer works with any image type that satisfies:

```cpp
template<typename Image>
concept PixeledImage = requires(Image image, int x, int y, Image::pixel_type pixel) {
    typename Image::pixel_type;
    { image.width() } -> std::convertible_to<int>;
    { image.height() } -> std::convertible_to<int>;
    { image.set_pixel(x, y, pixel) };
};
```

This completely separates fractal logic from the way images are stored or output.

---

### 🧾 Logging

Minimal thread-safe logger:

* Levels: `INFO`, `WARN`, `DEBUG`, `ERROR`
* Colored console output
* Formatting via `std::format`
* Mutex-protected

```cpp
LOG_INFO("Rendering started: {}x{}", width, height);
```

Can be fully disabled via `LOGGING_ENABLED` for performance-critical builds.

---

## 🛠 Technologies Used

* C++20/23
* Concepts
* Templates
* OpenMP
* CMake
* GoogleTest

---

## 🧪 Testing

Unit tests with **GoogleTest** cover:

* Math classes (Complex, Quaternion, Vec3)
* BMP image handling

---

## 📌 Architectural Highlights

✔ Separation of math, rendering, and output
✔ Minimal dependencies
✔ Generic design via concepts
✔ Extensible via higher-order functions
✔ Precise control over binary structures
✔ Suitable as a foundation for:

* Fractal visualizers
* CPU renderers
* Educational numerical methods projects

---

## ⚠ Limitations

* Only **24-bit uncompressed BMP** supported
* Structure alignment is little-endian oriented (typical x86)
* Rendering is CPU-based

---

## 📈 Future Directions

* GPU rendering (CUDA / OpenCL)
* Multithreaded tiling
* Interactive viewer with sliders (SDL / ImGui)
* 3D fractals using quaternions

# THANK YOU
