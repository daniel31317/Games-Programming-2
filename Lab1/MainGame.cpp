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

	m_mainCamera.initCamera(glm::vec3(0, 0.25, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);

	initGameObjects();
}

void MainGame::initGameObjects()
{
	//ground
	m_gameObjects[0]->GetTransform()->SetPosition(glm::vec3(0.0, -1.0, 0.0));	
	m_gameObjects[0]->GetTransform()->SetRotation(glm::vec3(glm::radians(90.0), 0.0, 0.0));
	m_gameObjects[0]->GetTransform()->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
	m_gameObjects[0]->SetShader(*m_shaderManager.GetShader(BUMP));
	m_gameObjects[0]->SetTexture(*m_textureManager.GetTexture(ROCK));
	m_gameObjects[0]->SetMesh(*m_meshManager.GetMesh(QUAD));


	//leclerc
	m_gameObjects[1]->GetTransform()->SetPosition(glm::vec3(0.0, -1.0, -1.5));
	m_gameObjects[1]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
	m_gameObjects[1]->GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	m_gameObjects[1]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[1]->SetTexture(*m_textureManager.GetTexture(NONE));
	m_gameObjects[1]->SetMesh(*m_meshManager.GetMesh(LECLERC));

	cameraOffset = m_mainCamera.GetPosition()  - *m_gameObjects[1]->GetTransform()->GetPosition();

	//surbaisse
	/*
	m_gameObjects[2]->GetTransform()->SetPosition(glm::vec3(1.1, -0.9, 0.0));
	m_gameObjects[2]->GetTransform()->SetRotation(glm::vec3(glm::radians(-5.0f), glm::radians(200.0f), 0.0));
	m_gameObjects[2]->GetTransform()->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
	m_gameObjects[2]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[2]->SetTexture(*m_textureManager.GetTexture(NONE));
	m_gameObjects[2]->SetMesh(*m_meshManager.GetMesh(AMX_50));*/
}

void MainGame::gameLoop()
{
	float fpsTimer = 0.0f;
	int fpsCounter = 0;
	std::string words = "Game Programming 2 | FPS: ";

	while (_gameState != GameState::EXIT)
	{
		float currentFrame = SDL_GetTicks() / 1000.0f;  // convert ms to seconds
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsTimer += deltaTime;
		fpsCounter++;
		if (fpsTimer >= 1.f)
		{
			std::string title = words + std::to_string(fpsCounter);
			SDL_SetWindowTitle(_gameDisplay.getWindow(), title.c_str());
			fpsTimer = 0.0f;
			fpsCounter = 0;
		}

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


	const Uint8* state = SDL_GetKeyboardState(NULL);
	float rotAmount = 1.f;
	float moveAmount = 5.f;

	if (state[SDL_SCANCODE_W])
	{
		m_gameObjects[1]->GetTransform()->move(m_gameObjects[1]->GetTransform()->GetForward() * moveAmount * deltaTime);
		m_mainCamera.move(m_mainCamera.GetForward() * moveAmount * deltaTime);
	}
	else if (state[SDL_SCANCODE_S])
	{
		m_gameObjects[1]->GetTransform()->move(-m_gameObjects[1]->GetTransform()->GetForward() * moveAmount * deltaTime);
		m_mainCamera.move(-m_mainCamera.GetForward() * moveAmount * deltaTime);
	}
	else if (state[SDL_SCANCODE_A])
	{
		m_gameObjects[1]->GetTransform()->rotate(glm::vec3(0.0, rotAmount * deltaTime, 0.0));
		m_mainCamera.rotate(rotAmount * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(m_gameObjects[1]->GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		m_mainCamera.SetPosition(*m_gameObjects[1]->GetTransform()->GetPosition() + rotatedOffset);
	}
	else if (state[SDL_SCANCODE_D])
	{
		m_gameObjects[1]->GetTransform()->rotate(glm::vec3(0.0, -rotAmount * deltaTime, 0.0));
		m_mainCamera.rotate(-rotAmount * deltaTime, glm::vec3(0, 1, 0));
		glm::mat4 rotMat = glm::rotate(m_gameObjects[1]->GetTransform()->GetRotation()->y, glm::vec3(0, 1, 0));
		glm::vec3 rotatedOffset = glm::vec3(rotMat * glm::vec4(cameraOffset, 0.0f));
		m_mainCamera.SetPosition(*m_gameObjects[1]->GetTransform()->GetPosition() + rotatedOffset);
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay();

	for(int i = 0; i < NUM_GAME_OBJECTS; i++)
	{
		m_gameObjects[i]->GetShader()->Bind();
		m_gameObjects[i]->GetShader()->Update(*m_gameObjects[i]->GetTransform(), m_mainCamera);
		m_gameObjects[i]->GetTexture()->Bind(0);
		m_gameObjects[i]->GetMesh()->draw();
	}	

	//m_mainCamera.move(glm::vec3(0.0f, 0.0f, 0.01f));
	//m_mainCamera.rotate(0.01, glm::vec3(0, 1, 0));


	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}



