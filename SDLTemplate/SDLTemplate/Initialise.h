#pragma once
#include "stdafx.h"
#include "Window.h"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>

class Initialise
{
public:

	SDL_Window* initaliseSDLWindow();
	SDL_Renderer* initaliseSDLRenderer();
	int initaliseSDL();
	SDL_GLContext initialiseContext(SDL_Window* mainWindow);
	bool SetOpenGLAttributes();
	int initaliseGlew(SDL_Window* mainWindow);

	Initialise();
	~Initialise();

private:
	Window window;
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* renderer = NULL;
	SDL_GLContext gl_Context = nullptr;
};

