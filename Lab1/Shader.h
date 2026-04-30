#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "Camera.h"
#include "Constants.h"

class Shader
{
public:

	Shader();
	Shader(const std::string& filename);

	void unload();

	void Bind(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Camera& camera, bool hasTexture);
	void Update(const Transform& transform, const Camera& camera, bool hasTexture, float deadProgression);

	std::string Shader::LoadShader(const std::string& fileName);

	void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	GLuint Shader::CreateShader(const std::string& text, unsigned int type);

	~Shader();


protected:

private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,
		MODEL_U,     
		CAMPOS_U,  
		VIEW_U,
		PROJECTION_U,
		CAMVIEW_U,   
		CAMPROJ_U,  
		DIFFUSE_U,
		LIGHTPOS_U,
		LIGHTCOLOUR_U,
		VERTEXCOLOUR_U,
		HASTEXTURE_U,
		NOISE_TEXTURE_U,
		DEAD_PROGRESS_U,
		NUM_UNIFORMS

	};

	GLuint program; // Track the shader program 
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables

};
