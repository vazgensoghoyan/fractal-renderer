# Interactive Fractal Explorer PRD

Build a desktop explorer where users edit a complex formula, adjust parameters, and navigate the result with mouse, keyboard, or numeric fields. Render fractals on the GPU using WebGPU.

## Stack and structure

| Component | Responsibility |
| --- | --- |
| C++23 | Application state, navigation, formula parsing and WGSL generation |
| SDL3 | Window, input events, resize and display scaling |
| wgpu-native and WGSL | GPU computation, textures and presentation |
| Dear ImGui | Text fields, sliders, numeric inputs, presets and errors |
| CMake and GoogleTest | Dependency configuration, builds and tests |

Follow the SDL3 and WebGPU wrapper structure in `~/Documents/univerity/comp-graphics/cpp/wgpu_app`. Pin compatible SDL3, wgpu-native and ImGui revisions. Use ImGui's SDL3 and WebGPU backends and standard uniform buffers; the course project's native `Immediates` extension is unnecessary.

Replace the raylib viewer with this stack. Keep math, BMP, CPU rendering and existing examples independent of the new desktop dependencies. Retain the CPU implementation for reference tests; automatic CPU fallback is outside the first release.

## First release

- Fractal canvas with a collapsible settings panel.
- Drag to pan; wheel to zoom around the cursor; arrows to pan; `+` and `-` to zoom around the center. Support numeric keypad equivalents. Disable navigation shortcuts while editing text.
- Editable view center (real and imaginary), viewport width, iteration limit, and escape radius. Mouse and keyboard navigation update these fields.
- Editable iteration formula, initial `z`, and parameter `c`; Mandelbrot and Julia presets.
- Named complex constants with numeric inputs and adjustable slider ranges for real and imaginary components.
- Palette selection, reset view, and previous view. One drag or continuous key press creates one history entry.
- Coarse preview followed by full-resolution rendering; new input supersedes unfinished work.

Defer animation, 3D, browser deployment, arbitrary executable code, transcendental functions, saved sessions and deep-zoom arithmetic. Start with GPU `f32`; indicate when zoom reaches coordinate precision limits instead of implying unlimited zoom.

## Formula contract

For each pixel, evaluate initial `z` and `c` once. Check `abs(z) > escape_radius` before each iteration; otherwise evaluate the iteration expression, up to the iteration limit.

| Field | Available variables | Mandelbrot | Julia |
| --- | --- | --- | --- |
| Initial z | `pixel`, constants | `0` | `pixel` |
| Parameter c | `pixel`, constants | `pixel` | `a` |
| Iteration | `z`, `c`, constants | `z*z+c` | `z*z+c` |

The Julia preset defines `a = -0.8 + 0.156*i`. Expressions support real literals, `i`, parentheses, unary signs, `+`, `-`, `*`, `/`, and integer powers. Define power precedence explicitly: `-z^2` means `-(z^2)`. Reject reserved or duplicate constant names and unknown variables.

Parse validated expressions into an immutable syntax tree, then generate WGSL with explicit complex-arithmetic helpers. Never insert raw user text into shader code. Bound expression size, nesting, exponent magnitude and iteration count.

Guard division and arithmetic ranges explicitly; do not rely on WGSL preserving NaN or infinity. Mark numerical failures with a distinct color. Use escape-count coloring initially; quadratic smoothing is not a universal default.

Formula edits create a new shader and pipeline after Enter or a 250 ms typing pause. Keep the last valid pipeline until replacement succeeds; display parser and GPU compilation errors beside the editor. Navigation and parameter-value changes update uniform buffers without recompilation. Changing constant declarations may rebuild the pipeline.

## Implementation steps

### 1. Establish the desktop foundation

- Add SDL3, wgpu-native and Dear ImGui targets; separate core and desktop dependencies in CMake.
- Implement window, device, surface, frame acquisition, resize and resource cleanup following `WgpuApp`. Handle asynchronous initialization, minimized windows and GPU errors.
- Display an ImGui panel with a text input and slider. Forward SDL events to ImGui and respect its input capture.

Done when the panel works, resizing and display scaling are correct, and closing releases GPU resources.

### 2. Render a fixed fractal

- Render Mandelbrot with a WGSL fragment shader over a fullscreen triangle into an offscreen texture.
- Supply camera, canvas dimensions, iterations and escape radius through uniform buffers. Represent complex numbers as pairs with complex-arithmetic helpers.
- Store escape counts and classification in a GPU result texture; color it in a separate pass and show the output in the ImGui canvas. Avoid CPU readback during interaction.

Done when a small deterministic render agrees with a CPU reference within precision-aware expectations and palette edits require only recoloring.

### 3. Add navigation and parameter controls

- Introduce shared view and render state. Implement drag, cursor-centered wheel zoom, keyboard controls, numeric editing, reset and view history.
- Map pointer coordinates relative to the canvas and account for display scaling. Handle one-pixel dimensions and skip empty canvases.
- Validate finite coordinates, positive finite viewport width and escape radius, and bounded positive iterations. Reject values that cannot be represented by the GPU path.
- Update sliders immediately; commit numeric text on Enter or after a valid 250 ms pause. Preserve the last valid state on errors.

Done when mouse, keyboard and numeric controls describe the same view; panel interactions never move the fractal.

### 4. Add formula editing

- Implement the formula contract, syntax tree, WGSL generator, named constants and presets.
- Validate the three expressions and constants as one snapshot. Collect shader diagnostics and cache successful pipelines by formula and constant layout.
- Use generation IDs so delayed compilation cannot activate an obsolete formula. Keep editing and the last valid view available while a replacement is prepared.
- Test parsing, precedence, scope, limits, complex operations and numerical guards. Compare generated Mandelbrot and Julia shaders against CPU reference samples away from unstable boundaries.

Done when `z*z+c` can become `z^3+c` without restarting; changing constant values needs no recompilation and invalid edits preserve the working view.

### 5. Keep exploration responsive and verify

- Render previews at one quarter of each canvas dimension, then refine at full resolution in bounded tile batches. Recompute only when relevant state changes.
- Keep queued GPU work bounded. Submitted work cannot simply be cancelled: stop scheduling obsolete batches and discard outdated results using generation IDs. Avoid blocking GPU waits in the UI loop.
- Tune formula and iteration limits against worst-case interior pixels. Target input response within 100 ms on the documented development machine; measure render and compilation latency separately.
- Test camera mapping, stale-result rejection and palette-only updates. Manually verify presets, custom formulas, focus, history, resize, precision limits and shutdown during rendering.
- Build all targets, run the full suite, and update CI plus both READMEs for dependencies, controls and expression syntax.

Done when supported workloads remain interactive, stale frames never replace the current view, and the existing math/BMP tests and CPU examples still work.
