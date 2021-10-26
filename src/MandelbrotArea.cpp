#include <gdkmm/general.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/eventcontrollerscroll.h>
#include <gtkmm/gesturedrag.h>
#include <iostream>

#include "Config.hpp"
#include "MandelbrotArea.hpp"

Glib::RefPtr<Gdk::Pixbuf> create_mandelbrot_buffer() {
	return Gdk::Pixbuf::create(Gdk::Colorspace::RGB, false, 8, Config::image_width, Config::image_height);
}

MandelbrotArea::MandelbrotArea()
	: buffer{ create_mandelbrot_buffer() }, drag_controller{ Gtk::GestureDrag::create() }, zoom_controller{ Gtk::EventControllerScroll::create() } {
	update_mandelbrot();
	zoom_controller->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
	zoom_controller->signal_scroll().connect(sigc::mem_fun(*this, &MandelbrotArea::scroll), false);
	add_controller(zoom_controller);
	drag_controller->signal_drag_begin().connect(sigc::mem_fun(*this, &MandelbrotArea::drag_start));
	drag_controller->signal_drag_update().connect(sigc::mem_fun(*this, &MandelbrotArea::drag_update));
	add_controller(drag_controller);
}

void MandelbrotArea::update_mandelbrot() {
	if (std::chrono::steady_clock::now() - debounce > std::chrono::milliseconds(5)) {
		mandelbrot.draw(*buffer);
		set(buffer);
		queue_draw();
		debounce = std::chrono::steady_clock::now();
	}
}

void MandelbrotArea::drag_update(double const x_offset, double const y_offset) {
	double const correction = mandelbrot.zoom_level / -192;
	Mandelbrot::coord_t const useful_offset{ x_offset * correction, y_offset * -correction };
	mandelbrot.zoom_center = before_drag_coord + useful_offset;
	update_mandelbrot();
}

void MandelbrotArea::drag_start(double, double) {
	before_drag_coord = mandelbrot.zoom_center;
}

bool MandelbrotArea::scroll(double const delta_x, double delta_y) {
	(void)delta_x;
	delta_y /= -10;
	delta_y += 1;
	mandelbrot.zoom_level /= delta_y;
	update_mandelbrot();
	return true;
}
