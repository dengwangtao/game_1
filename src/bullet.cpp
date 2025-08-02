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


    return 0;
}

s32 Bullet::UpdatePosition(s64 now_ms)
{
    LOG_DEBUG("Bullet::UpdatePosition");

    f64 dt = (now_ms - last_update_ms()) / 1000.0f;

    position_.x += static_cast<f32>(move_dir_x()) * speed() * dt;
    position_.y += static_cast<f32>(move_dir_y()) * speed() * dt;

    // 限制在窗口边界内
    position_.x = Tools::clamp<f32>(position_.x, 0.0f, G_GAME.window_width() - width());
    position_.y = Tools::clamp<f32>(position_.y, 0.0f, G_GAME.window_height() - height());

    return 0;
}