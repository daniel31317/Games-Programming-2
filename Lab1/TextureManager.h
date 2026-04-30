#pragma once
#include <memory>
#include "Texture.h"

enum TextureName
{
	MUZZLEFLASH,
	NOISE,
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

