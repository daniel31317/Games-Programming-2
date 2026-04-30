#pragma once
#include <memory>
#include "Shader.h"

enum ShaderName
{

	COLLIDEROUTLINE,
	ADS,
	UIELEMENT,
	REMOVE_BACKGRROUND,
	TANK,
	NUM_SHADERS
};


class ShaderManager
{
private:

	std::unique_ptr<Shader> m_shaderList[NUM_SHADERS];

public:

	ShaderManager();
	~ShaderManager();

	void LoadShaders();

	Shader* GetShader(ShaderName name);


};

