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
#include "Tank.h"
#include "ColliderEditor.h"
#include "EnemyTank.h"

enum class GameState { PLAY, EXIT };

#define NUM_GAME_OBJECTS 1
#define NUM_COLLIDERS 64





class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void initGameObjects();
	
	void gameLoop();

	void processInput();
	void update();
	void drawGame();

	Display _gameDisplay;
	GameState _gameState; 

	ShaderManager m_shaderManager;
	MeshManager m_meshManager;
	TextureManager m_textureManager;

	std::unique_ptr<GameObject> m_gameObjects[NUM_GAME_OBJECTS];


	std::unique_ptr<Tank> m_tank;

	std::unique_ptr<EnemyTank> m_EnemyTank;

	Camera m_mainCamera;


	std::unique_ptr <ColliderEditor> m_colliderEditor;


	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	int lastFrameCount = 0;

	bool freeCamera = false;	
	bool colliderEditorActive = false;	
	bool collidersShowing = false;	
	bool updateGameTitle = false;	

	bool wallHacks = false;

	bool hDown = false;
	bool cDown = false;

};
