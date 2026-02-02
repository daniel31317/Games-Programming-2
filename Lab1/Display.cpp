#include "Display.h"


Display::Display()
{
	_window = nullptr; //initialise to generate null access violation for debugging. 
	_screenWidth = 1024;
	_screenHeight = 768; 
}

Display::~Display()
{
}

void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Display::swapBuffer()
{
	SDL_GL_SwapWindow(_window); //swap buffers
}

void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	_window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL); // create window

	if (_window == nullptr)
	{
		returnError("window failed to create");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_window);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}