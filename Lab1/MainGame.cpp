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

	initGameObjects();

	skybox.initSkyBox();
	skybox.SetShader(m_shaderManager.GetShader(SKYBOX));

	SDL_SetRelativeMouseMode(SDL_TRUE);

	writeGameControls();
}

void MainGame::initGameObjects()
{

	m_mainCamera.initCamera(glm::vec3(0, 0.25, -5), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);

	//city
	m_gameObjects[0]->GetTransform()->SetPosition(glm::vec3(0.0, -1.0, 0.0));	
	m_gameObjects[0]->GetTransform()->SetRotation(glm::vec3(0.0, 0.0, 0.0));
	m_gameObjects[0]->GetTransform()->SetScale(glm::vec3(0.01, 0.01, 0.01));
	m_gameObjects[0]->SetShader(*m_shaderManager.GetShader(ADS));
	m_gameObjects[0]->SetTexture(*m_textureManager.GetTexture(CITYTEXTURE));
	m_gameObjects[0]->SetMesh(*m_meshManager.GetMesh(CITY_M));

	//make tanks and colliders in certain order so they all ahve their needed references
	m_tank = std::make_unique<Tank>(m_shaderManager, m_textureManager, m_meshManager, &m_mainCamera, true);

	m_EnemyTank = std::make_unique<EnemyTank>(m_shaderManager, m_textureManager, m_meshManager, &m_mainCamera, m_tank.get());

	m_colliderEditor = std::make_unique<ColliderEditor>(&m_shaderManager, &m_textureManager, &m_meshManager, m_tank.get(), m_EnemyTank->GetTank());

	m_tank->SetTankCollider(m_colliderEditor->GetTankCollider());

	m_EnemyTank->GetTank()->SetTankCollider(m_colliderEditor->GetEnemyTankCollider());
	m_EnemyTank->SetTankColliderOffset(m_colliderEditor->GetTankColliderOffset());
}

void MainGame::gameLoop()
{
	//fps stuff
	float fpsTimer = 0.0f;
	int fpsCounter = 0;
	std::string words = "Game Programming 2 | FPS: ";
	std::string words2 = " | Free Camera (TAB): ";
	std::string words3 = " | Collider Editor (LALT): ";

	std::string on = "ON";
	std::string off = "OFF";

	//update titlle to show fps if we havent exited
	//it also shows free camera and collider editor states
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



		//actual game loop
		processInput();
		update();
		drawGame();
	}
}


void MainGame::writeGameControls()
{
	std::system("cls");

	std::cout << "Move Forward - W\nMove Backward - S\n";
	std::cout << "Turn Left - A\nTurn Right - D\n";
	std::cout << "Look Around - Mouse\n";
	std::cout << "Sensitivity is a variable in MainGame.h if you need to change it\n";
	std::cout << "Shoot - Left Mouse\nZoom - Right Click\n";
	std::cout << "Respawn - R\nV-Sync Toggle - V\n";
	std::cout << "Toggle World Colliders - H\nX-Ray On Enemy - X\n";
	std::cout << "Toggle Free Camera - TAB\nToggle Collider Editor - LALT\n";
	std::cout << "Unlock/Lock Mouse - ESC\n";
	std::cout << "Fullscreen - F11\n";
}

void MainGame::writeFreeCamControls()
{
	std::system("cls");

	std::cout << "Move Forward - W\nMove Backward - S\n";
	std::cout << "Move Left - A\nMove Right - D\n";
	std::cout << "Move Up - LSHIFT\nMove Down - LCTRL\n";
	std::cout << "Look Around - Mouse\n";
	std::cout << "Sensitivity is a variable in MainGame.h if you need to change it\n";
	std::cout << "V-Sync Toggle - V\n";
	std::cout << "Toggle World Colliders - H\nX-Ray On Enemy - X\n";
	std::cout << "Toggle Free Camera - TAB\nToggle Collider Editor - LALT\n";
	std::cout << "Unlock/Lock Mouse - ESC\n";
	std::cout << "Fullscreen - F11\n";
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

				//switch to free cam and back
				if (event.key.keysym.sym == SDLK_TAB && !colliderEditorActive)
				{
					freeCamera = !freeCamera;
					if (!freeCamera)
					{
						writeGameControls();
						m_tank->ResetCameraToTank();
					}
					else
					{

						writeFreeCamControls();
						m_mainCamera.ResetPitch();
						m_mainCamera.SetRotation(m_mainCamera.GetRotation());
					}

					updateGameTitle = true;
				}


				//switch to collider editor and back
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
						writeGameControls();
						
					}
					updateGameTitle = true;
				}


				//show mouse or hide it
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


				//fullscreen
				if (event.key.keysym.sym == SDLK_F11) 
				{
					_gameDisplay.toggleFullscreen();

					int w, h;
					SDL_GetWindowSize(_gameDisplay.getWindow(), &w, &h);
					glViewport(0, 0, w, h);
				}


				break;


				//update projection if window changes size
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

	//zooming in
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

		//move crosshair
		m_tank->GetCrosshair()->GetTransform()->SetPosition(glm::vec3(0));


		//movement
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


		//mouse movement
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

		//movement
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
			m_tank->RotateHull(true, deltaTime, true, *m_colliderEditor->GetTankColliderOffset());
		}
		if (state[SDL_SCANCODE_D])
		{
			m_tank->RotateHull(false, deltaTime, true, *m_colliderEditor->GetTankColliderOffset());
		}


		//reset
		if (state[SDL_SCANCODE_R])
		{
			if (!rDown)
			{
				initGameObjects();
			}
		}
		else
		{
			rDown = false;
		}



		//just so sensitivity can be a normal number we divide by 1000.0 otherwise sense would be 0.001
		m_targetTurretAngle += (float)mouseX * (-m_sensitivity / 1000.f);

		m_targetBarrelPitch += (float)mouseY * (m_sensitivity / 1000.f);

		//update the turret aiming with the glm:;distance being to the nearest object to change crosshair
		m_tank->UpdateTurretAim(deltaTime, m_targetTurretAngle, m_targetBarrelPitch, isZooming, 
			glm::distance(*m_tank->GetBody()->GetTransform()->GetPosition(), m_colliderEditor->ShootDetection(*m_tank->GetBarrel()->GetTransform()->GetPosition(), m_tank->GetBarrel()->GetTransform()->GetForward(), true, false)));
		

		//perry target that man 500m away and fire
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			if (m_tank->GetIfCanShoot())
			{
				m_tank->Shoot();
				Transform* turretTransform = m_tank->GetTurret()->GetTransform();
				m_colliderEditor->ShootDetection(*m_tank->GetBarrel()->GetTransform()->GetPosition(), m_tank->GetBarrel()->GetTransform()->GetForward(), true, true);
			}
		}	
	}


	//hide colliders
	if (state[SDL_SCANCODE_H])
	{
		if (!hDown)
		{
			hDown = true;
			collidersShowing = !collidersShowing;
		}
		
	}
	else
	{
		hDown = false;
	}



	//give the player cheats because enemy is hard to find and it is kind of cracked if you dont know where it is
	if (state[SDL_SCANCODE_X])
	{
		if (!xDown)
		{
			xDown = true;
			wallHacks = !wallHacks;
		}
		
	}
	else
	{
		xDown = false;
	}



	//v-sync for testing delta time stuff
	if (state[SDL_SCANCODE_V])
	{
		if (!vDown)
		{
			vDown = true;
			_gameDisplay.toggleVSync();
		}
		
	}
	else
	{
		vDown = false;
	}
	
}


void MainGame::update()
{

	//update editor
	if (colliderEditorActive)
		m_colliderEditor->UpdateEditor();


	//tank collision before movement is probably not conventional and should probably be in a fixed updated or smth
	//but it works for this and if i had more time i wouldve 
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


	//other various updates
	m_EnemyTank->UpdateIfEnemyCanSeePlayer(m_colliderEditor->CheckForPlayer());

	m_tank->Update(deltaTime);

	m_EnemyTank->Update(deltaTime);

	m_mainCamera.UpdateZoom(deltaTime);
	
}


//draw game
void MainGame::drawGame()
{
	//clear
	_gameDisplay.clearDisplay();


	//skybox first
	skybox.Draw(m_mainCamera);


	//if we are not hiding the meshes in the editor
	if (!m_colliderEditor->GetHideMeshes())
	{
		//there is only one object in here but i made it so it was flexible if i needed more
		for (int i = 0; i < NUM_GAME_OBJECTS; i++)
		{
			m_gameObjects[i]->GetShader()->Bind();
			m_gameObjects[i]->GetShader()->Update(*m_gameObjects[i]->GetTransform(), m_mainCamera, true);
			m_gameObjects[i]->GetTexture()->Bind(0);
			m_gameObjects[i]->GetMesh()->draw();
		}

		//draw them tanks
		m_EnemyTank->GetTank()->Draw();
		m_tank->Draw();
	}
		
	//draw editor if we are editing or showing colliders
	if (colliderEditorActive || collidersShowing)
		m_colliderEditor->DrawEditor(m_mainCamera);

	
	//we are wall hacking 
	if (wallHacks)
	{
		m_colliderEditor->WallHacks(m_mainCamera);
	}


	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();

}

