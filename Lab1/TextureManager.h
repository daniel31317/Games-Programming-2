#pragma once
#include <memory>
#include "Texture.h"

enum TextureName
{
	BRICKS,
	WATER,
	NUM_TEXTURES
};


class TextureManager
{
private:

	std::unique_ptr<Texture> m_textureList[NUM_TEXTURES];

public:

	TextureManager();

	void LoadTextures();

	Texture* GetTexture(TextureName name);


};

