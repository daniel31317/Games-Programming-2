#include "MainGame.h"
#include <iostream>
#include <string>

unsigned int indices[] = { 0, 1, 2 };


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	for(int i = 0; i < NUM_GAME_OBJECTS; i++)
	{
		m_gameObjects[i] = std::make_unique<GameObject>();
	}
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

	initGameObjects();

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
}

void MainGame::initGameObjects()
{
	m_gameObjects[0]->GetTransform()->SetPosition(glm::vec3(0.0, -1.0, 0.0));	
	m_gameObjects[0]->GetTransform()->SetRotation(glm::vec3(glm::radians(90.0), 0.0, 0.0));
	m_gameObjects[0]->GetTransform()->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	m_gameObjects[0]->SetShader(*m_shaderManager.GetShader(BUMP));
	m_gameObjects[0]->SetTexture(*m_textureManager.GetTexture(ROCK));
	m_gameObjects[0]->SetMesh(*m_meshManager.GetMesh(QUAD));



	m_gameObjects[1]->GetTransform()->SetPosition(glm::vec3(-1.1, -0.95, 0.0));
	m_gameObjects[1]->GetTransform()->SetRotation(glm::vec3(glm::radians(-5.0f), glm::radians(160.f), 0.0));
	m_gameObjects[1]->GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	m_gameObjects[1]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[1]->SetTexture(*m_textureManager.GetTexture(NONE));
	m_gameObjects[1]->SetMesh(*m_meshManager.GetMesh(LECLERC));


	m_gameObjects[2]->GetTransform()->SetPosition(glm::vec3(1.1, -0.9, 0.0));
	m_gameObjects[2]->GetTransform()->SetRotation(glm::vec3(glm::radians(-5.0f), glm::radians(200.0f), 0.0));
	m_gameObjects[2]->GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	m_gameObjects[2]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[2]->SetTexture(*m_textureManager.GetTexture(NONE));
	m_gameObjects[2]->SetMesh(*m_meshManager.GetMesh(AMX_50));
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

	for(int i = 0; i < NUM_GAME_OBJECTS; i++)
	{
		m_gameObjects[i]->GetShader()->Bind();
		m_gameObjects[i]->GetShader()->Update(*m_gameObjects[i]->GetTransform(), myCamera);
		m_gameObjects[i]->GetTexture()->Bind(0);
		m_gameObjects[i]->GetMesh()->draw();
	}	

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}



