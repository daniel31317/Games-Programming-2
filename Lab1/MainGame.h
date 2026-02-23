#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <memory>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
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
	Mesh mesh;
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;
	Camera myCamera;

	float counter;

};
