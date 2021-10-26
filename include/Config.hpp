#pragma once

#include <cstddef>

#include "Mandelbrot.hpp"

namespace Config {
constexpr Mandelbrot::num_iterations_t num_iterations = 100;
constexpr double threshold = 2;

constexpr size_t image_width = 1080;
constexpr size_t image_height = 1080;
}  // namespace Config
