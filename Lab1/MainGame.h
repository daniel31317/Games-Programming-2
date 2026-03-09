#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <memory>
#include "Display.h" 
#include "ShaderManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "transform.h"
#include "Camera.h"
#include "Constants.h"

enum class GameState { PLAY, EXIT };


class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();

	Display _gameDisplay;
	GameState _gameState; 

	ShaderManager m_shaderManager;
	MeshManager m_meshManager;
	TextureManager m_textureManager;

	Camera myCamera;

	float counter;

};
