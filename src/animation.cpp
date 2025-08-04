#include "animation.h"
#include "texture_mgr.h"
#include "tools.h"
#include <sstream>

Animation::Animation(Scene* scene)
    : scene_(scene)
{
}

Animation::~Animation()
{
}

s32 Animation::init(const std::string& img_texture_path)
{
    return 0;
}

s32 Animation::update(s64 now_ms)
{
    return 0;
}

s32 Animation::render()
{
    return 0;
}

s32 Animation::makeTexture(const std::string& file_path)
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

SDL_FRect Animation::GetRect() const
{
    return SDL_FRect{
        static_cast<f32>(position().x),
        static_cast<f32>(position().y),
        static_cast<f32>(width()),
        static_cast<f32>(height())
    };
}

SDL_FPoint Animation::GetCenter() const
{
    return Tools::get_rect_center(GetRect());
}


std::string Animation::DebugString() const
{
    std::ostringstream oss;
    oss << "Animation: " << position().x << ", " << position().y << " size: " << width() << ", " << height();
    return oss.str();
}