#include "bullet.h"
#include "tools.h"
#include "game.h"

s32 Bullet::init(const std::string& img_texture_path)
{
    Object::init(img_texture_path);


    // 子弹设置移动方向
    if (spawner())
    {
        if (spawner()->IsPlayer())
        {
            // 玩家发射的子弹，向上发射
            set_move_dir_x(0);
            set_move_dir_y(-1);
        }
        else if (spawner()->IsEnemy())
        {
            // 敌人发射的子弹，向下发射
            set_move_dir_x(0);
            set_move_dir_y(1);
        }
    }

    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h);
    set_width(w);
    LOG_INFO("Bullet init: %s", DebugString().c_str());
    
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

    position_.x += move_dir_x() * speed() * dt;
    position_.y += move_dir_y() * speed() * dt;

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

s32 Bullet::onCollision(Object* other)
{
    if (! other)
    {
        return 0;
    }

    if (other->IsBullet())
    {
        return 0;
    }

    if (!other->IsPlayer() && !other->IsEnemy() && !other->IsShield())
    {
        return 0;
    }

    auto* self_spawner = originSpawner();
    auto* other_spawner = other->originSpawner();
    if (other_spawner == nullptr)
    {
        other_spawner = other;
    }

    if (self_spawner == nullptr)
    {
        LOG_ERROR("bullet:%s originSpawner is nullptr", DebugString().c_str());
        return -1;
    }

    if (self_spawner == other_spawner)
    {
        // 同一个对象发射的子弹，不处理
        return 0;
    }

    // 同类型对象不处理
    if (self_spawner->obj_type() == other_spawner->obj_type())
    {
        return 0;
    }

    auto old_hp = other->hp();

    // 子弹碰到其他对象，销毁
    set_hp(0);

    other->set_hp(other->hp() - attack());

    LOG_INFO("Bullet:%s hit Object:%s, hp: %d->%d", DebugString().c_str(), other->DebugString().c_str(), old_hp, other->hp());

    return 0;
}