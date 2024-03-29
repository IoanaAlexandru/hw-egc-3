#include "TextureManager.h"

#include <Core/GPU/Texture2D.h>
#include <Core/Managers/ResourcePath.h>
#include <include/utils.h>

using namespace std;

std::unordered_map<std::string, Texture2D*> TextureManager::mapTextures;
std::vector<Texture2D*> TextureManager::vTextures;

void TextureManager::Init() {
  LoadTexture(RESOURCE_PATH::TEXTURES, "default.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "white.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "black.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "noise.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "random.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "particle.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "crate.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "earth.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "grass_bilboard.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "bamboo.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "asphalt.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "grass.jpg");

  LoadTexture(RESOURCE_PATH::TEXTURES, "bark.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "bark.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "leaf.png");
  LoadTexture(RESOURCE_PATH::TEXTURES, "palm.png");

  LoadTexture(RESOURCE_PATH::TEXTURES, "building1.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "building2.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "building3.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "building4.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "building5.jpg");
  LoadTexture(RESOURCE_PATH::TEXTURES, "building6.jpg");
}

// TextureManager::~TextureManager()
//{
//	// delete textures
//	unsigned int size = (unsigned int) vTextures.size();
//	for (unsigned int i=0; i <size; ++i)
//		SAFE_FREE(vTextures[i]);
//}

Texture2D* TextureManager::LoadTexture(const string& path,
                                       const char* fileName) {
  Texture2D* texture = GetTexture(fileName);

  if (texture) {
    return mapTextures[fileName];
  }

  texture = new Texture2D();
  bool status = texture->Load2D((path + '/' + fileName).c_str(), GL_REPEAT);

  if (status == false) {
    delete texture;
    return vTextures[0];
  }

  vTextures.push_back(texture);
  mapTextures[fileName] = texture;
  return texture;
}

void TextureManager::SetTexture(string name, Texture2D* texture) {
  mapTextures[name] = texture;
}

Texture2D* TextureManager::GetTexture(const char* name) {
  if (mapTextures[name]) return mapTextures[name];
  return NULL;
}

Texture2D* TextureManager::GetTexture(unsigned int textureID) {
  if (textureID < vTextures.size()) return vTextures[textureID];
  return NULL;
}
