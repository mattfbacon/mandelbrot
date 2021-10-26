#include <cmath>
#include <iostream>

#include "Config.hpp"
#include "Mandelbrot.hpp"

using coord_t = Mandelbrot::coord_t;
using num_iterations_t = Mandelbrot::num_iterations_t;

// https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Optimized_escape_time_algorithms
num_iterations_t Mandelbrot::get_num_iterations(coord_t const coord) {
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	num_iterations_t i = 0;
	while (x2 + y2 <= 4 && i < Config::num_iterations) {
		y1 = 2 * x1 * y1 + coord.imag();
		x1 = x2 - y2 + coord.real();
		x2 = x1 * x1;
		y2 = y1 * y1;
		i++;
	}
	return i + (1 - log(log2(sqrt(x2 + y2))));
}

template <typename T>
using b = std::remove_const_t<T>;

template <typename T>
struct RGB {
	T r, g, b;
};
// https://www.codespeedy.com/hsv-to-rgb-in-cpp/
template <typename T>
RGB<T> hsv_to_rgb(double const H, double const s, double const v) {
	auto const C = s * v, X = C * (1 - abs(fmod(H / 60.0, 2) - 1)), m = v - C;
	RGB<double> ret;
	if (H >= 0 && H < 60) {
		ret.r = C, ret.g = X, ret.b = 0;
	} else if (H >= 60 && H < 120) {
		ret.r = X, ret.g = C, ret.b = 0;
	} else if (H >= 120 && H < 180) {
		ret.r = 0, ret.g = C, ret.b = X;
	} else if (H >= 180 && H < 240) {
		ret.r = 0, ret.g = X, ret.b = C;
	} else if (H >= 240 && H < 300) {
		ret.r = X, ret.g = 0, ret.b = C;
	} else {
		ret.r = C, ret.g = 0, ret.b = X;
	}
	return {
		(T)((ret.r + m) * 255),
		(T)((ret.g + m) * 255),
		(T)((ret.b + m) * 255),
	};
}

coord_t Mandelbrot::normalize_coords(size_t const row, size_t const col) {
	double y = Config::image_height - row;
	double x = col;
	// [0, image size]
	y /= Config::image_height / 2;
	x /= Config::image_width / 2;
	// [0, 2]
	y -= 1;
	x -= 1;
	// [-1, 1]
	y *= zoom_level;
	x *= zoom_level;
	// [-zoom, zoom]
	return coord_t{ x, y } + zoom_center;
}

void Mandelbrot::draw(Gdk::Pixbuf& buffer) {
	auto const buf_height = buffer.get_height();
	auto const buf_width = buffer.get_width();
	uint8_t* const pixels_ptr = buffer.get_pixels();
	auto const row_stride = buffer.get_rowstride();
	auto const n_channels = buffer.get_n_channels();
#pragma OMP parallel for collapse(2)
	for (b<decltype(buf_height)> row = 0; row < buf_height; row++) {
		for (b<decltype(buf_width)> col = 0; col < buf_width; col++) {
			uint8_t* const pixel = &pixels_ptr[row * row_stride + col * n_channels];
			auto const normalized_position = normalize_coords(row, col);
			auto const pixel_iterations = get_num_iterations(normalized_position);
			*((RGB<uint8_t>*)pixel) = hsv_to_rgb<uint8_t>((double)pixel_iterations / Config::num_iterations * 360, 1, pixel_iterations < Config::num_iterations);
		}
	}
}
