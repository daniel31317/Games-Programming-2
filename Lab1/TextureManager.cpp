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
	m_textureList[MUZZLEFLASH] = std::make_unique<Texture>("..\\res\\muzzleFlash.png");
	m_textureList[NOISE] = std::make_unique<Texture>("..\\res\\Perlin_noise.png");
	m_textureList[CITYTEXTURE] = std::make_unique<Texture>("..\\res\\City_Final_Texture.png");
	m_textureList[T80HULL_T] = std::make_unique<Texture>("..\\res\\T_80_HullTexture.png");
	m_textureList[T80TURRET_T] = std::make_unique<Texture>("..\\res\\T_80_TurretTexture.png");
	m_textureList[LECLERCHULL_T] = std::make_unique<Texture>("..\\res\\Leclerc_Hull_Texture.png");
	m_textureList[LECLERCTURRET_T] = std::make_unique<Texture>("..\\res\\Leclerc_Turret_Texture.png");
	m_textureList[LECLERCBARREL_T] = std::make_unique<Texture>("..\\res\\Leclerc_Barrel_Texture.png");
	m_textureList[NONE] = std::make_unique<Texture>("..\\res\\White_Color.jpg");
}

Texture* TextureManager::GetTexture(TextureName name)
{
	return m_textureList[name].get();
}