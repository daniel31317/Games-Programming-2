#include <iostream>
#include <fstream>
#include "Shader.h"


Shader::Shader()
{

}
	
Shader::Shader(const std::string& filename)
{

	program = glCreateProgram();

	//load shaders

	shaders[0] = CreateShader(LoadShader(filename + ".vert"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadShader(filename + ".frag"), GL_FRAGMENT_SHADER);


	//attach the shaders to the program
	for (int i = 0; i < sizeof(shaders) / sizeof(shaders[0]); i++)
	{
		glAttachShader(program, shaders[i]);
	}

	//bind the attribute location
	glBindAttribLocation(program, 0, "VertexPosition");
	glBindAttribLocation(program, 1, "UV"); 
	glBindAttribLocation(program, 2, "Normals");
	glBindAttribLocation(program, 3, "VertexColour");

	//link the program and check it linked
	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	//validate the shader program
	glValidateProgram(program); //check the entire program is valid
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");
	uniforms[MODEL_U] = glGetUniformLocation(program, "modelMatrix");
	uniforms[CAMPOS_U] = glGetUniformLocation(program, "camPos");
	uniforms[RIMCOLOUR_U] = glGetUniformLocation(program, "rimColour");
	uniforms[RIMPOWER_U] = glGetUniformLocation(program, "rimPower");
	uniforms[FOGCOLOUR_U] = glGetUniformLocation(program, "fogColour");
	uniforms[DIFFUSE_U] = glGetUniformLocation(program, "diffuse");
	uniforms[NORMALT_U] = glGetUniformLocation(program, "normalT");
	uniforms[LIGHTPOS_U] = glGetUniformLocation(program, "lightPos");
	uniforms[LIGHTCOLOUR_U] = glGetUniformLocation(program, "lightColour");
	uniforms[VERTEXCOLOUR_U] = glGetUniformLocation(program, "vertexColour");
	uniforms[HASTEXTURE_U] = glGetUniformLocation(program, "hasTexture");
}

Shader::~Shader()
{


}


void Shader::unload()
{
	for (int i = 0; i < sizeof(shaders) / sizeof(shaders[0]); i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteProgram(program);
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

void Shader::Bind()
{
	glUseProgram(program);
}

void Shader::Update(const Transform& transform, const Camera& camera, bool hasTexture)
{
	glm::mat4 model = transform.GetModel();
	glm::mat4 mvp = camera.GetViewProjection() * model;
	glm::vec3 cameraPosition = camera.GetPosition();

	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &mvp[0][0]);

	glUniformMatrix4fv(uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);

	glUniform3f(uniforms[CAMPOS_U], cameraPosition.x, cameraPosition.y, cameraPosition.z);

	glUniform3f(uniforms[RIMCOLOUR_U], 1.0f, 0.0f, 1.0f);

	glUniform1f(uniforms[RIMPOWER_U], 3.0f);

	glUniform3f(uniforms[FOGCOLOUR_U], backgroundColour.r, backgroundColour.g, backgroundColour.b);

	glUniform1i(uniforms[DIFFUSE_U], 0);

	glUniform1i(uniforms[NORMALT_U], 1);

	glUniform3f(uniforms[LIGHTPOS_U], 0.0f, 200.0f, 0.0f);

	glUniform3f(uniforms[LIGHTCOLOUR_U], 1.0f, 1.0f, 1.0f);

	glUniform3f(uniforms[VERTEXCOLOUR_U], 75.f / 255.f, 83.f / 255.f, 32.f / 255.f);

	glUniform1i(uniforms[HASTEXTURE_U], hasTexture);
}

