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
		
	}





	Tank* GetTank() { return &m_tank; }

private:

	Tank m_tank;

};