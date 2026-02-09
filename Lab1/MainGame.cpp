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

	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
	Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
	Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)) };
	
	


	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	Mesh mesh1(vertices, sizeof(vertices) / sizeof(vertices[0]));

	Shader shader("..\\res\\shader"); //new shader

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\Water.jpg"); //load texture



	Transform transform;
	transform.SetPos(glm::vec3(sinf(counter), 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(sinf(counter), sinf(counter), sinf(counter)));

	shader.Bind();
	shader.Update(transform);
	texture.Bind(0);
	mesh.Draw();

	counter += 0.01f;

	transform.SetPos(glm::vec3(-sinf(counter), 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));

	shader.Update(transform);
	texture1.Bind(0);
	mesh.Draw();

	glEnableClientState(GL_COLOR_ARRAY);

	glEnd();

	_gameDisplay.swapBuffer();
}



