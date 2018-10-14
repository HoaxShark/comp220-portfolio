// Main.cpp : Defines the entry point for the console application.

#include "Main.h"

int initaliseSDL();
int initaliseGlew();
bool SetOpenGLAttributes();
int initialiseContext();
bool isFullscreen = false;
vec3 position = vec3(0.0f);
vec3 shapeScale = vec3(1.0f);

//Game loop runs while true
bool gameRunning = true;

//The SDL renderer. Should be passed to class functions that handle their own rendering.
SDL_Renderer* renderer = NULL;

//The main game window.
SDL_Window* mainWindow = nullptr;

//
SDL_GLContext gl_Context = nullptr;


int main(int argc, char *argv[])
{
	bool inDevelopMode = false;

	//Initalise random seed
	std::srand(time(NULL));


	//Initialise times
	float lastTime = 0;
	float tickTime = 0;
	float deltaTime = 0;


	//Initalise the SDL components
	if (initaliseSDL() < 0)
	{
		std::cout << "SDL initalisation failed." << std::endl;
		return -1;
	}

	//Initalise OpenGL 
	SetOpenGLAttributes();
	initialiseContext();
	initaliseGlew();

	//Create Vertex Array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vert.glsl", "frag.glsl");

	// create view matrix
	mat4 ViewMatrix = translate(mat4(), vec3(-3.0f, 0.0f, 0.0f));

	/*glm::mat4 CameraMatrix = glm::lookAt(
		cameraPosition, // the position of your camera, in world space
		cameraTarget,   // where you want to look at, in world space
		upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);*/

	// set modelMatrix location
	GLuint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");

	//Current sdl event
	SDL_Event event;

	/*------------------------
	Main game loop
	------------------------*/
	while (gameRunning)
	{
		//Calculate deltaTime
		lastTime = tickTime;
		tickTime = SDL_GetTicks();
		deltaTime = (tickTime - lastTime);

		//Check for SDL events
		if (SDL_PollEvent(&event))
		{
			//Events found
			switch (event.type)
			{
					//Window closed
				case SDL_QUIT:
					gameRunning = false;
					break;

				case SDL_KEYDOWN:

					//Check individual keys by code (can be moved out into main switch statement if fewer keys need to be checked.)
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							gameRunning = false;
							break;

						case SDLK_UP:
							position = position + vec3(0.0f, 0.01f, 0.0f);
							break;

						case SDLK_LEFT:
							position = position + vec3(-0.01f, 0.0f, 0.0f);
							break;

						case SDLK_RIGHT:
							position = position + vec3(0.01f, 0.0f, 0.0f);
							break;

						case SDLK_DOWN:
							position = position + vec3(0.0f, -0.01f, 0.0f);
							break;

						case SDLK_7:
							shapeScale = shapeScale + vec3(-0.01f, -0.01f, 0.0f);
							break;

						case SDLK_8:
							shapeScale = shapeScale + vec3(0.01f, 0.01f, 0.0f);
							break;

						case SDLK_F11:
							
							//bool isFullscreen = Window.getIsFullscreen();
							if (isFullscreen) {
								isFullscreen = false;
								SDL_SetWindowFullscreen(mainWindow, 0);
							}
							else {
								isFullscreen = true;
								SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
							}
							break;
					}

				break;
			}
		}

		//Update game and render with openGL
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind program
		glUseProgram(programID);

		mat4 modelMatrix = translate(position);
		modelMatrix = glm::scale(modelMatrix, shapeScale);

		// sends across modelMatrix
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(mainWindow);
	}

	//Delete Program
	glDeleteProgram(programID);
	//Delete Buffer
	glDeleteBuffers(1, &vertexbuffer);
	//Delete Vertex Array
	glDeleteVertexArrays(1, &VertexArrayID);
	//Delete context
	SDL_GL_DeleteContext(gl_Context);
	//Close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}

int initaliseGlew()
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

bool SetOpenGLAttributes()
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

int initialiseContext()
{
	SDL_GLContext gl_Context = SDL_GL_CreateContext(mainWindow);
	if (gl_Context == nullptr)
	{
		std::cout << "Context initalisation failed." << std::endl;

		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		return -1;
	}
	return 0;
}
/*------------------------------------------------
SDL Initialisation functions.
Can be largely left alone.
Currently use screen dimensions set in globals.h.
------------------------------------------------*/

int initaliseSDL()
{
	//initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Cannot initalise SDL " << SDL_GetError() << std::endl;
		return -1;
	}

	//Initalise the main window
	mainWindow = SDL_CreateWindow(global::WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, global::SCREEN_WIDTH, global::SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if (mainWindow == nullptr)
	{
		std::cout << "Cannot create window " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	//Initalise renderer
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
		std::cout << "Cannot initalise SDL" << SDL_GetError << std::endl;
		SDL_Quit();
		return -1;
	}

	return 0;
}

