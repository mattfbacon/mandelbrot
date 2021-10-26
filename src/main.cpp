#include <gtkmm/application.h>

#include "Mandelbrot.hpp"
#include "Window.hpp"

int main(int const argc, char* argv[]) {
	auto app = Gtk::Application::create("be.mattfellenz.mandelbrot.main");
	return app->make_window_and_run<Window>(argc, argv);
}
