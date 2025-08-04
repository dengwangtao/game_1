#include "texture_mgr.h"
#include "game.h"

TextureManager::~TextureManager()
{
    for (auto& pair : textures_)
    {
        LOG_INFO("Destroying texture: %s", pair.first.c_str());
        SDL_DestroyTexture(pair.second);
    }
}

SDL_Texture *TextureManager::LoadTexture(const std::string &filename)
{
    if (textures_.find(filename)!= textures_.end())
    {
        return textures_[filename];
    }

    auto* t = IMG_LoadTexture(G_GAME.renderer(), filename.c_str());
    if (t == nullptr)
    {
        LOG_ERROR("Failed to load texture: %s", filename.c_str());
        return nullptr;
    }

    textures_[filename] = t;
    return t;
}