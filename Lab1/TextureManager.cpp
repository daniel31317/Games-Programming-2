#include "TextureManager.h"

TextureManager::TextureManager()
{

}

void TextureManager::LoadTextures()
{
	m_textureList[BRICKS] = std::make_unique<Texture>("..\\res\\bricks.jpg");
	m_textureList[WATER] = std::make_unique<Texture>("..\\res\\Water.jpg");
}

Texture* TextureManager::GetTexture(TextureName name)
{
	return m_textureList[name].get();
}