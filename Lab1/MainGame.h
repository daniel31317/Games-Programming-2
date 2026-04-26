#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <memory>
#include "Display.h" 
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "transform.h"
#include "GameObject.h"
#include "Camera.h"
#include "Constants.h"

enum class GameState { PLAY, EXIT };

#define NUM_GAME_OBJECTS 3

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void initGameObjects();
	void processInput();
	void gameLoop();
	void drawGame();

	Display _gameDisplay;
	GameState _gameState; 

	ShaderManager m_shaderManager;
	MeshManager m_meshManager;
	TextureManager m_textureManager;

	std::unique_ptr<GameObject> m_gameObjects[NUM_GAME_OBJECTS];

	Camera m_mainCamera;

	glm::vec3 cameraOffset = glm::vec3();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool freeCamera = false;	

};
