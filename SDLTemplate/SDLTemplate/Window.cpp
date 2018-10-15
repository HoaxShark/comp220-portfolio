#include "Window.h"

Window::Window()
{
	screenWidth = 600;
	screenHeight = 400;
	windowTitle = "Main Window";
}


Window::~Window()
{
}

bool Window::getIsFullscreen()
{
	return isFullscreen;
}

void Window::setIsFullscreen()
{
	isFullscreen = !isFullscreen;
}
