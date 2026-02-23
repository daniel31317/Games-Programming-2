#include "Display.h"


Display::Display()
{
	_sdlWindow = nullptr; //initialise to generate null access violation for debugging. 
	_glContext = nullptr;
	_screenWidth = 1024.0f;
	_screenHeight = 768.0f; 
}


Display::~Display()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_sdlWindow);
	SDL_Quit();
}

void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;

	std::cout << "Press any key to quit" << std::endl;

	std::cin.get();

	SDL_Event e;

	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}

void Display::swapBuffer()
{
	SDL_GL_SwapWindow(_sdlWindow);
}

void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   
	_sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)_screenWidth, (int)_screenHeight, SDL_WINDOW_OPENGL);
	_glContext = SDL_GL_CreateContext(_sdlWindow);


	GLenum error = glewInit();

	if (_sdlWindow == nullptr)
	{
		returnError("Window is null");
		return;
	}

	if (_glContext == nullptr)
	{
		returnError("Context is null");
		return;
	}

	if (error != GLEW_OK)
	{
		returnError("Glew Init failed");
		return;
	}
	
	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

}



void Display::clearDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void Display::clearDisplay(glm::vec4 backgroundColour)
{
	glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, backgroundColour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}


float Display::getWidth()
{
	return _screenWidth;
}

float Display::getHeight()
{
	return _screenHeight;
}