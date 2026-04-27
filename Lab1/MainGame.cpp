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
	m_tank = nullptr;
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

	m_tank = std::make_unique<Tank>(m_shaderManager, m_textureManager, m_meshManager, &m_mainCamera);
}

void MainGame::gameLoop()
{
	float fpsTimer = 0.0f;
	int fpsCounter = 0;
	std::string words = "Game Programming 2 | FPS: ";
	std::string words2 = " | Free Camera (TAB): ";

	std::string on = "ON";
	std::string off = "OFF";

	while (_gameState != GameState::EXIT)
	{
		float currentFrame = SDL_GetTicks() / 1000.0f;  // convert ms to seconds
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsTimer += deltaTime;
		fpsCounter++;
		if (fpsTimer >= 1.f)
		{
			lastFrameCount = fpsCounter;
			std::string title = words + std::to_string(fpsCounter) + words2;
			if(freeCamera)
				title += on;
			else
				title += off;
			SDL_SetWindowTitle(_gameDisplay.getWindow(), title.c_str());
			fpsTimer = 0.0f;
			fpsCounter = 0;
		}
		else if(updateGameTitle)
		{
			std::string title = words + std::to_string(lastFrameCount) + words2;
			if (freeCamera)
				title += on;
			else
				title += off;
			SDL_SetWindowTitle(_gameDisplay.getWindow(), title.c_str());

			updateGameTitle = false;
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

			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_TAB)
				{
					freeCamera = !freeCamera;
					if (!freeCamera)
					{
						m_tank->ResetCameraToTank();
					}

					updateGameTitle = true;
				}
				break;

			default: break;
		}
	}


	const Uint8* state = SDL_GetKeyboardState(NULL);
	float rotAmount = 1.f;
	float moveAmount = 5.f;



	if (freeCamera)
	{
		if (state[SDL_SCANCODE_W])
		{
			m_mainCamera.move(m_mainCamera.GetForward() * moveAmount * deltaTime);
		}
		if (state[SDL_SCANCODE_S])
		{
			m_mainCamera.move(-m_mainCamera.GetForward() * moveAmount * deltaTime);
		}
		if (state[SDL_SCANCODE_A])
		{
			m_mainCamera.rotate(rotAmount * deltaTime, glm::vec3(0, 1, 0));

		}
		if (state[SDL_SCANCODE_D])
		{
			m_mainCamera.rotate(-rotAmount * deltaTime, glm::vec3(0, 1, 0));
		}
		if(state[SDL_SCANCODE_LSHIFT])
		{
			m_mainCamera.move(glm::vec3(0, moveAmount * deltaTime, 0));
		}
		if(state[SDL_SCANCODE_LCTRL])
		{
			m_mainCamera.move(glm::vec3(0, -moveAmount * deltaTime, 0));
		}
	}
	else
	{
		if (state[SDL_SCANCODE_W])
		{
			m_tank->MoveForward(deltaTime);
		}
		if (state[SDL_SCANCODE_S])
		{
			m_tank->MoveBackwards(deltaTime);
		}
		if (state[SDL_SCANCODE_A])
		{
			m_tank->RotateBodyLeft(deltaTime);
		}
		if (state[SDL_SCANCODE_D])
		{
			m_tank->RotateBodyRight(deltaTime);
		}
		if (state[SDL_SCANCODE_LEFT])
		{
			m_tank->RotateTurretLeft(deltaTime);
		}
		if (state[SDL_SCANCODE_RIGHT])
		{
			m_tank->RotateTurretRight(deltaTime);
		}
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

	m_tank->Draw();



	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}



