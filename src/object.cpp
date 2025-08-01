#include "object.h"
#include <SDL.h>
#include <sstream>

Object::~Object()
{
    if (texture())
    {
        LOG_INFO("Destroy texture");
        SDL_DestroyTexture(texture());
    }
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