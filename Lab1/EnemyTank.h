#pragma once
#include "Tank.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

struct EnemyTank
{

public:

	EnemyTank() {}

	EnemyTank(ShaderManager& shaderManager, TextureManager& textureManager, MeshManager& meshManager, Camera* mainCamera) : m_tank(shaderManager, textureManager, meshManager, mainCamera, false)
	{
		m_tank.SetPosition(glm::vec3(2.0, -0.5, -1.5));
	}


	void Update(float deltaTime)
	{
		//m_tank.MoveForward();
		m_tank.Update(deltaTime);
	}



	Tank* GetTank() { return &m_tank; }

private:

	Tank m_tank;

};