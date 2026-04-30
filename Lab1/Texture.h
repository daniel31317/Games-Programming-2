#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture();
	Texture(const std::string& fileName);

	void unload();

	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture();

protected:
private:

	GLuint textureHandler;
};
