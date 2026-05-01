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
	m_shaderList[UIELEMENT] = std::make_unique<Shader>("..\\res\\UIElement");
	m_shaderList[REMOVE_BACKGRROUND] = std::make_unique<Shader>("..\\res\\RemoveBackground");
	m_shaderList[ADS] = std::make_unique<Shader>("..\\res\\ADS");
	m_shaderList[SKYBOX] = std::make_unique<Shader>("..\\res\\Skybox");
}

Shader* ShaderManager::GetShader(ShaderName name)
{
	return m_shaderList[name].get();
}