#include "texture_mgr.h"
#include "game.h"

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