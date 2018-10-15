#pragma once
#include <SDL.h>
#include <iostream>

class Window
{
public:
	Window();
	~Window();

	// if the window is currently fullscreen
	bool isFullscreen;

	//Dimensions of SDL window.
	int screenWidth;
	int screenHeight;

	//Window Title
	char* windowTitle;

	// get and set isFullscreen
	bool getIsFullscreen();
	void setIsFullscreen();

};

