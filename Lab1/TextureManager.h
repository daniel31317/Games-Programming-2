#pragma once
#include <memory>
#include "Texture.h"

enum TextureName
{
	BRICKS,
	WATER,
	ROCK,
	MUZZLEFLASH,
	CITYTEXTURE,
	NONE,
	NUM_TEXTURES
};


class TextureManager
{
private:

	std::unique_ptr<Texture> m_textureList[NUM_TEXTURES];

public:

	TextureManager();
	~TextureManager();

	void LoadTextures();

	Texture* GetTexture(TextureName name);


};

