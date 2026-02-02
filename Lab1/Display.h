#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
#include <utility>


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearDisplay();

private:

	SDL_Window* _window; //holds pointer to out window
	int _screenWidth;
	int _screenHeight;

	SDL_GLContext _context;


	void returnError(std::string errorString);


	
};

