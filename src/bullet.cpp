#include "bullet.h"
#include "tools.h"
#include "game.h"

s32 Bullet::init(const std::string& img_texture_path)
{
    Object::init(img_texture_path);


    // 子弹设置移动方向
    if (spawner() && spawner()->IsPlayer())
    {
        // 玩家发射的子弹，向上发射
        set_move_dir_x(0);
        set_move_dir_y(-1);
    }

    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h);
    set_width(w);
    
    return 0;
}

s32 Bullet::update(s64 now_ms)
{
    Object::update(now_ms);

    return 0;
}

s32 Bullet::UpdatePosition(s64 now_ms)
{
    f64 dt = (now_ms - last_update_ms()) / 1000.0f;
    if (last_update_ms() == 0)
    {
        dt = 0.0f;
    }

    position_.x += static_cast<f32>(move_dir_x()) * speed() * dt;
    position_.y += static_cast<f32>(move_dir_y()) * speed() * dt;

    // 边界检测
    auto this_rect = GetRect();
    auto game_rect = SDL_FRect{0, 0, static_cast<f32>(G_GAME.window_width()), static_cast<f32>(G_GAME.window_height())};
    SDL_FRect result;
    bool is_intersect = SDL_IntersectFRect(&this_rect, &game_rect, &result);
    if (! is_intersect)
    {
        LOG_INFO("Bullet out of game"); // 销毁
        scene()->markRemoveObject(this);
    }

    return 0;
}