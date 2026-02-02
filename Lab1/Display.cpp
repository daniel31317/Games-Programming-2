#include "Display.h"


Display::Display()
{
	_window = nullptr; //initialise to generate null access violation for debugging. 
	_context = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768; 
}


Display::~Display()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
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
	SDL_GL_SwapWindow(_window);
}

void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   
	_window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	_context = SDL_GL_CreateContext(_window);
	GLenum error = glewInit();

	if (_window == nullptr)
	{
		returnError("Window is null");
		return;
	}

	if (_context == nullptr)
	{
		returnError("Context is null");
		return;
	}

	if (error != GLEW_OK)
	{
		returnError("Glew Init failed");
		return;
	}
	
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

}



void Display::clearDisplay()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}