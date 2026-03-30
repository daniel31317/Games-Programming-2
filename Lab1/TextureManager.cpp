#include "TextureManager.h"

TextureManager::TextureManager()
{

}

void TextureManager::LoadTextures()
{
	m_textureList[BRICKS] = std::make_unique<Texture>("..\\res\\bricks.jpg");
	m_textureList[WATER] = std::make_unique<Texture>("..\\res\\water.jpg");
	m_textureList[ROCK] = std::make_unique<Texture>("..\\res\\rock.jpg");
	m_textureList[NONE] = std::make_unique<Texture>("..\\res\\White_Color.jpg");
}

Texture* TextureManager::GetTexture(TextureName name)
{
	return m_textureList[name].get();
}