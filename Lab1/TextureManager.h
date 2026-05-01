#pragma once
#include <memory>
#include "Texture.h"

enum TextureName
{

	//https://pngtree.com/freepng/front-gun-shots-muzzle-flash-fire-effects_8749538.html
	MUZZLEFLASH,

	//https://en.wikipedia.org/wiki/Perlin_noise
	NOISE,

	//baked myself from blender
	CITYTEXTURE,
	T80HULL_T,
	T80TURRET_T,
	LECLERCHULL_T,
	LECLERCTURRET_T,
	LECLERCBARREL_T,

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

