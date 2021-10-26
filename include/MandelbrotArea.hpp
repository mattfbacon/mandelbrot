#pragma once

#include <chrono>
#include <gtkmm/image.h>

#include "Mandelbrot.hpp"

namespace Gdk {
class Pixbuf;
}
namespace Gtk {
class GestureDrag;
class EventControllerScroll;
}  // namespace Gtk

class MandelbrotArea : public Gtk::Image {
public:
	MandelbrotArea();
	void update_mandelbrot();
protected:
	Mandelbrot mandelbrot;
	Glib::RefPtr<Gdk::Pixbuf> buffer;
protected:
	Glib::RefPtr<Gtk::GestureDrag> drag_controller;
	Glib::RefPtr<Gtk::EventControllerScroll> zoom_controller;
	Mandelbrot::coord_t before_drag_coord;
	std::chrono::time_point<std::chrono::steady_clock> debounce;
public:
	void drag_start(double, double);
	void drag_update(double, double);
	bool scroll(double, double);
};
