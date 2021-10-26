#pragma once

#include <gtkmm/window.h>

#include "MandelbrotArea.hpp"

class Window : public Gtk::Window {
public:
	Window();
	void draw_mandelbrot();
protected:
	MandelbrotArea area;
	void update_mandelbrot();
};
