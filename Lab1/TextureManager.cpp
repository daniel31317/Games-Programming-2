#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for(int i = 0; i < NUM_TEXTURES; i++)
	{
		m_textureList[i]->unload();
	}
}

void TextureManager::LoadTextures()
{
	m_textureList[BRICKS] = std::make_unique<Texture>("..\\res\\bricks.jpg");
	m_textureList[WATER] = std::make_unique<Texture>("..\\res\\water.jpg");
	m_textureList[ROCK] = std::make_unique<Texture>("..\\res\\rock.jpg");
	m_textureList[MUZZLEFLASH] = std::make_unique<Texture>("..\\res\\muzzleFlash.png");
	m_textureList[CITYTEXTURE] = std::make_unique<Texture>("..\\res\\City_Final_Texture.png");
	m_textureList[NONE] = std::make_unique<Texture>("..\\res\\White_Color.jpg");
}

Texture* TextureManager::GetTexture(TextureName name)
{
	return m_textureList[name].get();
}