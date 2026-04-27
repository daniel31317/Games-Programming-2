#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	
}

void ShaderManager::LoadShaders()
{
	m_shaderList[FOG] = std::make_unique<Shader>("..\\res\\fog");
	m_shaderList[RIM_LIGHT] = std::make_unique<Shader>("..\\res\\rimlight");
	m_shaderList[TOON] = std::make_unique<Shader>("..\\res\\toon");
	m_shaderList[BUMP] = std::make_unique<Shader>("..\\res\\bump");
	m_shaderList[REMOVE_BACKGRROUND] = std::make_unique<Shader>("..\\res\\RemoveBackground");
	m_shaderList[ADS] = std::make_unique<Shader>("..\\res\\ADS");
}

Shader* ShaderManager::GetShader(ShaderName name)
{
	return m_shaderList[name].get();
}