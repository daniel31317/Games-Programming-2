#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	
}

ShaderManager::~ShaderManager()
{
	for(int i = 0; i < NUM_SHADERS; i++)
	{
		m_shaderList[i]->unload();
	}
}

void ShaderManager::LoadShaders()
{
	m_shaderList[COLLIDEROUTLINE] = std::make_unique<Shader>("..\\res\\ColliderOutline");
	m_shaderList[TANK] = std::make_unique<Shader>("..\\res\\Tank");
	m_shaderList[REMOVE_BACKGRROUND] = std::make_unique<Shader>("..\\res\\RemoveBackground");
	m_shaderList[ADS] = std::make_unique<Shader>("..\\res\\ADS");
}

Shader* ShaderManager::GetShader(ShaderName name)
{
	return m_shaderList[name].get();
}