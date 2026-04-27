#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>


Texture::Texture()
{

}
Texture::Texture(const std::string& fileName)
{
	int width, height, numberOfComponents;

	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numberOfComponents, 4);

	if (imageData == NULL)
	{
		std::cerr << "Texture failed to load " << fileName << std::endl;
	}

	//number of and address of textures
	glGenTextures(1, &textureHandler);

	//bind texture - define type & specify texture we are working with
	glBindTexture(GL_TEXTURE_2D, textureHandler);

	// wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// wrap texture outside height
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	// linear filtering for magnification (texture is larger)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); 

	stbi_image_free(imageData);
}

Texture::~Texture()
{
	//number of and address of textures
	glDeleteTextures(1, &textureHandler);
}


void Texture::Bind(unsigned int unit)
{
	//check we are working with one of the 32 textures
	assert(unit >= 0 && unit <= 31); 

	//set acitve texture unit
	glActiveTexture(GL_TEXTURE0 + unit); 

	//type of and texture to bind to unit
	glBindTexture(GL_TEXTURE_2D, textureHandler);

}