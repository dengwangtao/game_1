#include "object.h"
#include <SDL.h>
#include <sstream>
#include "tools.h"
#include "game.h"
#include "guid_gen.h"
#include "scene.h"

Object::Object(Scene* scene)
    : guid_(GUIDGen::GenerateGUID(GUID_TYPE::GUID_PLAYER))
    , scene_(scene)
{
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

s32 Object::update(s64 now_ms)
{

    UpdatePosition(now_ms);


    set_last_update_ms(now_ms);
    return 0;
}

SDL_Rect Object::GetRect() const
{
    return SDL_Rect{
        static_cast<int>(position().x),
        static_cast<int>(position().y),
        static_cast<int>(width()),
        static_cast<int>(height())
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