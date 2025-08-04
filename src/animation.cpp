#include "animation.h"
#include "texture_mgr.h"
#include "tools.h"
#include "game.h"
#include <sstream>

Animation::Animation(Scene* scene)
    : scene_(scene)
{
    LOG_INFO("Animation created");
}

Animation::~Animation()
{
    LOG_INFO("Animation destroyed: %s", DebugString().c_str());
}

s32 Animation::init(const std::string& img_texture_path)
{
    s32 ret = makeTexture(img_texture_path);
    if (ret)
    {
        LOG_ERROR("Failed to load texture");
        return ret;
    }

    // 获取到纹理的宽高
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);

    s32 frame_count = w / h; // 整除
    if (frame_count <= 0)
    {
        frame_count = 1;
    }
    set_frame_count(frame_count);


    set_height(h);
    set_width(h); // 正方形

    LOG_INFO("Animation init: %s", DebugString().c_str());



    return 0;
}

s32 Animation::play(s64 duration_ms)
{
    if (started())
    {
        return 0;
    }

    s64 now_ms = G_GAME.now_ms();

    set_start_time_ms(now_ms);
    set_duration_ms(duration_ms);

    set_started(true);

    LOG_INFO("Animation play: %s", DebugString().c_str());

    return 0;
}

s32 Animation::update(s64 now_ms)
{
    s64 dt = now_ms - start_time_ms();

    if (started())
    {
        s64 frame_single_duration_ms = duration_ms() / frame_count();

        if (frame_single_duration_ms <= 0)
        {
            frame_single_duration_ms = 1;
        }

        s32 cur_frame = dt / frame_single_duration_ms;

        if (cur_frame >= frame_count())
        {
            cur_frame = frame_count() - 1;
            set_started(false);
        }
        set_current_frame(cur_frame);
    }

    if (dt > duration_ms())
    {
        set_started(false);
        scene()->markRemoveAnimation(this);
    }

    return 0;
}

s32 Animation::render()
{
    SDL_Rect src_rect = GetSrcRect();
    SDL_FRect rect = GetRect();
    s32 ret = 0;
    ret = SDL_RenderCopyF(G_GAME.renderer(), texture(), &src_rect, &rect);

    if (ret)
    {
        LOG_ERROR("Failed to render object: %s", SDL_GetError());
        return ret;
    }

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

SDL_Rect Animation::GetSrcRect() const
{
    return SDL_Rect{
        static_cast<s32>(current_frame() * width()),
        0,
        static_cast<s32>(width()),
        static_cast<s32>(height())
    };
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