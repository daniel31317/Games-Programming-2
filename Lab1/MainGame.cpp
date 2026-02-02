#include "MainGame.h"
#include <iostream>
#include <string>

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT: _gameState = GameState::EXIT; 
				SDL_Quit();	 
			break;

			default: break;
		}
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay();

	Vertex vertices[3] = { Vertex(glm::vec3(-0.5, -0.5, 0)),
						   Vertex(glm::vec3(0, 0.5, 0)),
						   Vertex(glm::vec3(0.5, -0.5, 0)) };

	Mesh mesh(vertices, 3);
	Shader shader("shader");
	shader.Bind();
	mesh.Draw();
	

	_gameDisplay.swapBuffer();
}


