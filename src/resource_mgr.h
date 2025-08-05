#pragma once

#include "singleton.h"
#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <unordered_map>

class ResourceManager : public Singleton<ResourceManager>
{
public:
    ~ResourceManager();
    SDL_Texture* LoadTexture(const std::string& filename);

private:
    std::unordered_map<std::string, SDL_Texture*> textures_;
};

#define G_RESOURCE_MGR ResourceManager::Instance()