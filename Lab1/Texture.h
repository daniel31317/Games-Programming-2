#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);

	void CreateBlankTexture();

	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture();

protected:
private:

	GLuint textureHandler;
};
