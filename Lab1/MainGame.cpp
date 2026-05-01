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

	m_colliderEditor = std::make_unique<ColliderEditor>();
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
	
	m_textureManager.LoadTextures();

	m_meshManager.LoadMeshes();

	m_mainCamera.initCamera(glm::vec3(0, 0.25, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);

	initGameObjects();

	skybox.initSkyBox();
	skybox.SetShader(m_shaderManager.GetShader(SKYBOX));

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void MainGame::initGameObjects()
{
	//city
	m_gameObjects[0]->GetTransform()->SetPosition(glm::vec3(0.0, -1.0, 0.0));	
	m_gameObjects[0]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
	m_gameObjects[0]->GetTransform()->SetScale(glm::vec3(0.01, 0.01, 0.01));
	m_gameObjects[0]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[0]->SetTexture(*m_textureManager.GetTexture(CITYTEXTURE));
	m_gameObjects[0]->SetMesh(*m_meshManager.GetMesh(CITY));

	//crosshair
	m_gameObjects[1]->GetTransform()->SetPosition(glm::vec3(0.0, 0.0, 0.0));
	m_gameObjects[1]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
	m_gameObjects[1]->GetTransform()->SetScale(glm::vec3(1.0, 1.0, 1.0));
	m_gameObjects[1]->SetShader(*m_shaderManager.GetShader(UIELEMENT));
	m_gameObjects[1]->SetTexture(*m_textureManager.GetTexture(NONE));
	m_gameObjects[1]->SetMesh(*m_meshManager.GetMesh(CROSSHAIR));


	m_tank = std::make_unique<Tank>(m_shaderManager, m_textureManager, m_meshManager, &m_mainCamera, true);

	m_EnemyTank = std::make_unique<EnemyTank>(m_shaderManager, m_textureManager, m_meshManager, &m_mainCamera, m_tank.get());

	m_colliderEditor = std::make_unique<ColliderEditor>(&m_shaderManager, &m_textureManager, &m_meshManager, m_tank.get(), m_EnemyTank->GetTank());

	m_tank->SetTankCollider(m_colliderEditor->GetTankCollider());

	m_EnemyTank->GetTank()->SetTankCollider(m_colliderEditor->GetEnemyTankCollider());
	m_EnemyTank->SetTankColliderOffset(m_colliderEditor->GetTankColliderOffset());
}

void MainGame::gameLoop()
{
	float fpsTimer = 0.0f;
	int fpsCounter = 0;
	std::string words = "Game Programming 2 | FPS: ";
	std::string words2 = " | Free Camera (TAB): ";
	std::string words3 = " | Collider Editor (LALT): ";

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


			title += words3;

			if (colliderEditorActive)
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


			title += words3;

			if (colliderEditorActive)
				title += on;
			else
				title += off;


			SDL_SetWindowTitle(_gameDisplay.getWindow(), title.c_str());

			updateGameTitle = false;
		}

		processInput();
		update();
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
				if (event.key.keysym.sym == SDLK_TAB && !colliderEditorActive)
				{
					freeCamera = !freeCamera;
					if (!freeCamera)
					{
						m_tank->ResetCameraToTank();
					}
					else
					{
						m_mainCamera.ResetPitch();
						m_mainCamera.SetRotation(m_mainCamera.GetRotation());
					}

					updateGameTitle = true;
				}

				if(event.key.keysym.sym == SDLK_LALT)
				{
					colliderEditorActive = !colliderEditorActive;
					if (colliderEditorActive)
					{

						if (!freeCamera)
						{
							m_mainCamera.ResetPitch();
							m_mainCamera.SetRotation(m_mainCamera.GetRotation());
						}

						m_colliderEditor->OpenEditor();

						freeCamera = true;
											
					}
					else
					{
						freeCamera = false;
						m_tank->ResetCameraToTank();
						m_colliderEditor->CloseEditor();
						
					}
					updateGameTitle = true;
				}



				if(event.key.keysym.sym == SDLK_ESCAPE)
				{					
					
					SDL_bool mouse = SDL_GetRelativeMouseMode();

					if (mouse == SDL_TRUE)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);
					}
					else
					{
						SDL_SetRelativeMouseMode(SDL_TRUE);
					}
								
				}

				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					int width = event.window.data1;
					int height = event.window.data2;

					// Update OpenGL viewport
					glViewport(0, 0, width, height);

					// Update camera projection
					m_mainCamera.updateProjection(70.0f, (float)width / height, 0.01f, 1000.0f);
				}
				break;

			default: break;
		}
	}


	const Uint8* state = SDL_GetKeyboardState(NULL);
	float rotAmount = 2.f;
	float moveAmount = 10.f;

	




	int mouseX, mouseY;

	//mouseState
	Uint32 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);


	if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		m_mainCamera.SetZooming(true);
	}
	else
	{
		m_mainCamera.SetZooming(false);
	}


	isZooming = m_mainCamera.GetZooming();


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
			m_mainCamera.move(-m_mainCamera.GetRight() * moveAmount * deltaTime);

		}
		if (state[SDL_SCANCODE_D])
		{
			m_mainCamera.move(m_mainCamera.GetRight() * moveAmount * deltaTime);
		}
		if(state[SDL_SCANCODE_LSHIFT])
		{
			m_mainCamera.move(m_mainCamera.GetUp() * moveAmount * deltaTime);
		}
		if(state[SDL_SCANCODE_LCTRL])
		{
			m_mainCamera.move(-m_mainCamera.GetUp() * moveAmount * deltaTime);
		}


		if (mouseX != 0)
		{
			float yawAmount = (float)mouseX * m_sensitivity / 1000.f;
			m_mainCamera.rotate(-yawAmount, glm::vec3(0, 1, 0));
		}

		if (mouseY != 0)
		{
			float pitchAmount = (float)mouseY * m_sensitivity / 1000.f;
			float currentPitch = m_mainCamera.GetPitch();


			if (pitchAmount > 0)
			{
				if (currentPitch > glm::radians(-89.0f))
				{
					m_mainCamera.rotate(-pitchAmount, m_mainCamera.GetRight());
					m_mainCamera.AddPitch(-pitchAmount);
				}
			}
			else 
			{
				if (currentPitch < glm::radians(89.0f))
				{
					m_mainCamera.rotate(-pitchAmount, m_mainCamera.GetRight());
					m_mainCamera.AddPitch(-pitchAmount);
				}
			}
		}


	}
	else
	{
		if (state[SDL_SCANCODE_W])
		{
			m_tank->MoveForward();
		}
		if (state[SDL_SCANCODE_S])
		{
			m_tank->MoveBackwards();
		}
		if (state[SDL_SCANCODE_A])
		{
			m_tank->RotateBodyLeft(deltaTime, true, *m_colliderEditor->GetTankColliderOffset());
		}
		if (state[SDL_SCANCODE_D])
		{
			m_tank->RotateBodyRight(deltaTime, true, *m_colliderEditor->GetTankColliderOffset());
		}



		//just so sensitivity can be a normal number
		m_targetTurretAngle += (float)mouseX * (-m_sensitivity / 1000.f);

		m_tank->UpdateTurretAim(deltaTime, m_targetTurretAngle, isZooming);


		if(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (m_tank->GetIfCanShoot())
			{
				m_tank->Shoot();
				Transform* turretTransform = m_tank->GetTurret()->GetTransform();
				m_colliderEditor->ShootDetection(*turretTransform->GetPosition(), turretTransform->GetForward(), true);
			}	
		}



		

	}

	if (state[SDL_SCANCODE_H])
	{
		if (!hDown)
		{
			hDown = true;
			collidersShowing =! collidersShowing;
		}
		
	}
	else
	{
		hDown = false;
	}


	if (state[SDL_SCANCODE_C])
	{
		if (!cDown)
		{
			cDown = true;
			wallHacks = !wallHacks;
		}
		
	}
	else
	{
		cDown = false;
	}
	
}


void MainGame::update()
{
	if (colliderEditorActive)
		m_colliderEditor->UpdateEditor();



	std::array<bool, 2> tankCollision = m_colliderEditor->CollisionDetection();
	
	//handle if tanks crash into each other
	if (tankCollision[1])
	{
		m_tank->HandleColliison(tankCollision[1], 4);

		m_EnemyTank->GetTank()->HandleColliison(tankCollision[1], 4);
	}
	else
	{
		m_tank->HandleColliison(tankCollision[0], 8);
	}

	m_EnemyTank->UpdateIfEnemyCanSeePlayer(m_colliderEditor->CheckForPlayer());

	m_tank->Update(deltaTime);

	//m_EnemyTank->Update(deltaTime);

	m_mainCamera.UpdateZoom(deltaTime);
	
}



void MainGame::drawGame()
{
	_gameDisplay.clearDisplay();

	if (!m_colliderEditor->GetHideMeshes())
	{
		for (int i = 0; i < NUM_GAME_OBJECTS; i++)
		{
			m_gameObjects[i]->GetShader()->Bind();
			m_gameObjects[i]->GetShader()->Update(*m_gameObjects[i]->GetTransform(), m_mainCamera, true);
			m_gameObjects[i]->GetTexture()->Bind(0);
			m_gameObjects[i]->GetMesh()->draw();
		}

		m_tank->Draw();

		m_EnemyTank->GetTank()->Draw();
	}
		
	if (colliderEditorActive || collidersShowing)
		m_colliderEditor->DrawEditor(m_mainCamera);

	skybox.Draw(m_mainCamera);

	if (wallHacks)
	{
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		m_colliderEditor->WallHacks(m_mainCamera);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}


	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}



