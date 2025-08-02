#include "object.h"
#include <SDL.h>
#include <sstream>
#include "tools.h"
#include "game.h"
#include "guid_gen.h"
#include "scene.h"
#include <SDL.h>
#include <SDL_image.h>
#include "texture_mgr.h"


Object::Object(ObjectType objtype, Scene *scene, Object* spawner)
    : obj_type_(objtype)
    , scene_(scene)
    , spawner_(spawner)
{
    set_guid(GUIDGen::GenerateGUID(obj_type()));
    LOG_INFO("Create object: guid=%s", GUIDGen::ParseGUID(guid()).c_str());
}

Object::~Object()
{
    LOG_INFO("Destroy object: guid=%s", GUIDGen::ParseGUID(guid()).c_str());

    if (texture())
    {
        LOG_INFO("Destroy texture");
        SDL_DestroyTexture(texture());
    }
}

s32 Object::init(const std::string &img_texture_path)
{
    s32 ret = makeTexture(img_texture_path);
    if (ret)
    {
        LOG_ERROR("Failed to load texture");
        return ret;
    }

    return 0;
}

s32 Object::update(s64 now_ms)
{

    UpdatePosition(now_ms);


    set_last_update_ms(now_ms);
    return 0;
}

s32 Object::render()
{
    SDL_Rect rect = GetRect();
    s32 ret = SDL_RenderCopy(G_GAME.renderer(), texture(), NULL, &rect);
    if (ret)
    {
        LOG_ERROR("Failed to render object: %s", SDL_GetError());
        return ret;
    }
    return 0;
}

s32 Object::makeTexture(const std::string &file_path)
{
    set_texture(
        G_TEXTURE_MGR.LoadTexture(file_path)
    );

    if (texture() == nullptr)
    {
        LOG_ERROR("Failed to load texture");
        return -1;
    }
    return 0;
}
SDL_Rect Object::GetRect() const
{
    return SDL_Rect{
        static_cast<s32>(position().x),
        static_cast<s32>(position().y),
        static_cast<s32>(width()),
        static_cast<s32>(height())
    };
}

std::string Object::DebugString() const
{
    std::ostringstream oss;
    oss << "Object: " << this << " pos: " << position().x << "," << position().y << " size: " << width() << "," << height();
    return oss.str();
}

s32 Object::UpdatePosition(s64 now_ms)
{
    return 0;
}