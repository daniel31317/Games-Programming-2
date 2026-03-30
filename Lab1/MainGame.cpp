#include "MainGame.h"
#include <iostream>
#include <string>

unsigned int indices[] = { 0, 1, 2 };
Transform transform;


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

	m_shaderManager.LoadShaders();
	m_meshManager.LoadMeshes();
	m_textureManager.LoadTextures();

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
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

	transform.SetPos(glm::vec3(0.0, -1.0, 0.0));
	transform.SetRot(glm::vec3(glm::radians(90.0), 0.0, 0.0));
	transform.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

	m_shaderManager.GetShader(BUMP)->Bind();
	m_shaderManager.GetShader(BUMP)->Update(transform, myCamera);

	m_textureManager.GetTexture(ROCK)->Bind(0);

	m_meshManager.GetMesh(QUAD)->draw();
	
	
	
	transform.SetPos(glm::vec3(-1.1, -0.95, 0.0));
	//transform.SetRot(glm::vec3(glm::radians(-5.0f), glm::radians(160.f), 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	m_shaderManager.GetShader(ADS)->Bind();
	m_shaderManager.GetShader(ADS)->Update(transform, myCamera);

	m_textureManager.GetTexture(NONE)->Bind(0);

	m_meshManager.GetMesh(LECLERC)->draw();
	
	
	transform.SetPos(glm::vec3(1.1, -0.9, 0.0));
	//transform.SetRot(glm::vec3(glm::radians(-5.0f), glm::radians(200.0f), 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.3f, 0.3f, 0.3f));

	m_shaderManager.GetShader(ADS)->Bind();
	m_shaderManager.GetShader(ADS)->Update(transform, myCamera);

	m_meshManager.GetMesh(AMX_50)->draw();

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}



