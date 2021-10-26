#pragma once

#include <complex>
#include <cstddef>
#include <gdkmm/pixbuf.h>

struct Mandelbrot {
	using num_iterations_t = double;
	using coord_t = std::complex<double>;
protected:
	template <typename T>
	static T complex_abs(std::complex<T> const value) {
		return sqrt((value.real() * value.real()) + (value.imag() * value.imag()));
	}
	/**
	 * @param coord - the coordinate in the image to calculate for
	 * @param recursive_input - the result of the recursive quadratic map
	 * This would be passed back into the calculate function to recurse.
	 */
	static coord_t next_iteration(coord_t coord, coord_t recursive_input = { 0, 0 });
	/**
	 * @returns how many iterations it takes for the threshold to be reached.
	 * will return Config::num_iterations if it does not exit the threshold in this time.
	 */
	static num_iterations_t get_num_iterations(coord_t coord);
public:
	coord_t zoom_center{ 0, 0 };
	double zoom_level = 2.0;
	coord_t normalize_coords(size_t row, size_t col);
	void draw(Gdk::Pixbuf& buffer);
};
