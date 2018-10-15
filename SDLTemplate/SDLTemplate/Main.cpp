// Main.cpp : Defines the entry point for the console application.

#include "Main.h"

vec3 position = vec3(0.0f);
vec3 shapeScale = vec3(1.0f);

// Create Initialisation class
Initialise init;

// Create window class for managing fullscreen
Window window;

//Game loop runs while true
bool gameRunning = true;

//The SDL renderer. Should be passed to class functions that handle their own rendering.
SDL_Renderer* renderer = NULL;

//The main game window.
SDL_Window* mainWindow = nullptr;

// Create context for openGL
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
	mainWindow = init.initaliseSDLWindow();
	renderer = init.initaliseSDLRenderer();

	//Initalise OpenGL 
	init.SetOpenGLAttributes();
	gl_Context = init.initialiseContext(mainWindow);
	init.initaliseGlew(mainWindow);

	//Create Vertex Array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// x,y,z,r,g,b,a
//change to use the vertex header
	static const Vertex t[] =
	{
		{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f},
		{0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f},
	};

	static const Vertex v[] =
	{
		{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f},
		{0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f},
	};

	static const int indice[] =
	{
		0,1,2,
		2,0,3
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL. Change to 
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), v, GL_STATIC_DRAW);
	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vert.glsl", "frag.glsl");

	// create elemenet buffer variable
	GLuint elementbuffer;
	// set buffer location in memory
	glGenBuffers(1, &elementbuffer);
	// bind the buffer (bound until you unbind it, or bind something else)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	// call to copy the data, array type, size of data 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indice, GL_STATIC_DRAW);

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
						// Exit the game
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
							
							// switch between fullscreen and window
							if (window.getIsFullscreen()) {
								window.setIsFullscreen();
								SDL_SetWindowFullscreen(mainWindow, 0);
							}
							else {
								window.setIsFullscreen();
								SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
							}
							break;
					}

				break;
			}
		}

		//Update game and render with openGL
		glClearColor(0.0, 0.5, 0.0, 1.0);
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
		glVertexAttribPointer
		(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(Vertex),                  // stride
			(void*)0            // array buffer offset
		);

		// binds the vertex 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer
		(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(3 * sizeof(float))
		);

		// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(mainWindow);
	}
	//Delete Program
	glDeleteProgram(programID);
	//Delete Buffer
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &elementbuffer);
	//Delete Vertex Array
	glDeleteVertexArrays(1, &VertexArrayID);
	//Delete context
	SDL_GL_DeleteContext(gl_Context);
	//Close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}