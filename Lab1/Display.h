#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
using namespace std;


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearDisplay(float r, float g, float b, float a);
	void clearDisplay(glm::vec4 backgroundColour);

	float getWidth();
	float getHeight();

private:

	void returnError(std::string errorString);

	SDL_GLContext _glContext; //global variable to hold the context
	SDL_Window* _sdlWindow; //holds pointer to out window
	float _screenWidth;
	float _screenHeight;

};
