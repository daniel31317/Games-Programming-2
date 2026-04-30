#pragma once
#include "Tank.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include <array>

struct EnemyTank
{

public:

	EnemyTank() {}

	EnemyTank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* mainCamera) : m_tank(shaderManager, textureManager, meshManager, mainCamera, false)
	{

		int indexX = 4;
		int indexZ = 5;


		m_tank.SetPosition(glm::vec3(movementGrid[indexX][indexZ].x, -0.5, -movementGrid[indexX][indexZ].y));
	}


	void Update(float deltaTime)
	{
		//m_tank.MoveForward();
		m_tank.Update(deltaTime);
	}



	Tank* GetTank() { return &m_tank; }

private:

	Tank m_tank;

	//really cursed layout but hey
	//bottom left (x,z)	(0,0)																																																//top left				
	const std::array<std::array<glm::vec2, 8>, 9> movementGrid = { {
	{ glm::vec2(99.5f, 101.5f),  glm::vec2(99.5f, 78.5f),  glm::vec2(99.5f, 55.5f),  glm::vec2(99.5f, 33.0f),  glm::vec2(99.5f, 10.0f),  glm::vec2(99.5f, -13.5f),  glm::vec2(99.5f, -35.5f),  glm::vec2(99.5f, -57.0f)  },
	{ glm::vec2(75.0f, 101.5f),  glm::vec2(75.0f, 78.5f),  glm::vec2(75.0f, 55.5f),  glm::vec2(75.0f, 33.0f),  glm::vec2(75.0f, 10.0f),  glm::vec2(75.0f, -13.5f),  glm::vec2(75.0f, -35.5f),  glm::vec2(75.0f, -57.0f)  },
	{ glm::vec2(50.0f, 101.5f),  glm::vec2(50.0f, 78.5f),  glm::vec2(50.0f, 55.5f),  glm::vec2(50.0f, 33.0f),  glm::vec2(50.0f, 10.0f),  glm::vec2(50.0f, -13.5f),  glm::vec2(50.0f, -35.5f),  glm::vec2(50.0f, -57.0f)  },
	{ glm::vec2(25.0f, 101.5f),  glm::vec2(25.0f, 78.5f),  glm::vec2(25.0f, 55.5f),  glm::vec2(25.0f, 33.0f),  glm::vec2(25.0f, 10.0f),  glm::vec2(25.0f, -13.5f),  glm::vec2(25.0f, -35.5f),  glm::vec2(25.0f, -57.0f)  },
	{ glm::vec2(-0.5f, 101.5f),  glm::vec2(-0.5f, 78.5f),  glm::vec2(-0.5f, 55.5f),  glm::vec2(-0.5f, 33.0f),  glm::vec2(-0.5f, 10.0f),  glm::vec2(-0.5f, -13.5f),  glm::vec2(-0.5f, -35.5f),  glm::vec2(-0.5f, -57.0f)  },
	{ glm::vec2(-25.0f, 101.5f), glm::vec2(-25.0f, 78.5f), glm::vec2(-25.0f, 55.5f), glm::vec2(-25.0f, 33.0f), glm::vec2(-25.0f, 10.0f), glm::vec2(-25.0f, -13.5f), glm::vec2(-25.0f, -35.5f), glm::vec2(-25.0f, -57.0f) },
	{ glm::vec2(-49.0f, 101.5f), glm::vec2(-49.0f, 78.5f), glm::vec2(-49.0f, 55.5f), glm::vec2(-49.0f, 33.0f), glm::vec2(-49.0f, 10.0f), glm::vec2(-49.0f, -13.5f), glm::vec2(-49.0f, -35.5f), glm::vec2(-49.0f, -57.0f) },
	{ glm::vec2(-76.0f, 101.5f), glm::vec2(-76.0f, 78.5f), glm::vec2(-76.0f, 55.5f), glm::vec2(-76.0f, 33.0f), glm::vec2(-76.0f, 10.0f), glm::vec2(-76.0f, -13.5f), glm::vec2(-76.0f, -35.5f), glm::vec2(-76.0f, -57.0f) },
	{ glm::vec2(-99.0f, 101.5f), glm::vec2(-99.0f, 78.5f), glm::vec2(-99.0f, 55.5f), glm::vec2(-99.0f, 33.0f), glm::vec2(-99.0f, 10.0f), glm::vec2(-99.0f, -13.5f), glm::vec2(-99.0f, -35.5f), glm::vec2(-99.0f, -57.0f) }
} };
	//bottom right																																																	//top right

};