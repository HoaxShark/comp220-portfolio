#pragma once
#include <SDL.h>
#include <iostream>

class Window
{
public:
	Window();
	~Window();

	bool isFullscreen;

	bool getIsFullscreen();
	void setIsFullscreen();

};

