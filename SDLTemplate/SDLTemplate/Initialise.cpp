#include "Initialise.h"

Initialise::Initialise()
{
	initaliseSDL();
}


Initialise::~Initialise()
{
}


int Initialise::initaliseSDL() 
{
	//initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Cannot initalise SDL " << SDL_GetError() << std::endl;
		return -1;
	}
	return 0;
}

SDL_Window * Initialise::initaliseSDLWindow()
{
	//Initalise the main window
	mainWindow = SDL_CreateWindow(window.windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window.screenWidth, window.screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (mainWindow == nullptr)
	{
		std::cout << "Cannot create window " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}

	return mainWindow;
}

SDL_Renderer * Initialise::initaliseSDLRenderer()
{
	//Initalise renderer
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
		std::cout << "Cannot initalise SDL" << SDL_GetError << std::endl;
		SDL_Quit();
		return nullptr;
	}

	return renderer;
}

SDL_GLContext Initialise::initialiseContext(SDL_Window * mainWindow)
{
	SDL_GLContext gl_Context = SDL_GL_CreateContext(mainWindow);
	if (gl_Context == nullptr)
	{
		std::cout << "Context initalisation failed." << std::endl;

		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		return nullptr;
	}
	return gl_Context;
}

bool Initialise::SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

int Initialise::initaliseGlew(SDL_Window * mainWindow)
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Glew initalisation failed. Error:" << glewGetErrorString(err) << std::endl;

		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
		return -1;
	}
	return 0;
}
