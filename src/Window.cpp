#include "Window.hpp"

Window::Window() {
	set_title("Mandelbrot");
	set_child(area);
}
