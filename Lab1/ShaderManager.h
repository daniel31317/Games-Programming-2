#pragma once
#include <memory>
#include "Shader.h"

enum ShaderName
{
	FOG,
	RIM_LIGHT,
	TOON,
	BUMP,
	ADS,
	REMOVE_BACKGRROUND,
	NUM_SHADERS
};


class ShaderManager
{
private:

	std::unique_ptr<Shader> m_shaderList[NUM_SHADERS];

public:

	ShaderManager();

	void LoadShaders();

	Shader* GetShader(ShaderName name);


};

