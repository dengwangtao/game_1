#include "shield.h"
#include "scene.h"
#include "player.h"
#include "tools.h"
#include <SDL.h>

s32 Shield::init(const std::string &img_texture_path)
{
    Object::init(img_texture_path);

    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h / 3);
    set_width(w / 3);
    LOG_INFO("Shield init: %s", DebugString().c_str());

    // 设置hp
    set_hp(3);

    return 0;
}

s32 Shield::update(s64 now_ms)
{
    Object::update(now_ms);

    return s32();
}

s32 Shield::onCollision(Object *other)
{
    if (! other)
    {
        return 0;
    }

    if (other->IsEnemy())
    {
        LOG_INFO("Shield:%s collides with Enemy:%s", DebugString().c_str(), other->DebugString().c_str());

        other->set_hp(0);
        set_hp(hp() - other->attack());
    }

    return 0;
}

s32 Shield::UpdatePosition(s64 now_ms)
{
    auto* p = originSpawner();
    if (! p)
    {
        scene()->markRemoveObject(this);
        return 0;
    }

    auto a_rect = SDL_FRect{
        p->position().x, p->position().y - height(),
        static_cast<f32>(p->width()), static_cast<f32>(height())
    };
    auto born_pos = Tools::calculate_aligned_position(a_rect, width(), height());
    position_.x = born_pos.x;
    position_.y = born_pos.y;

    return 0;
}

s32 Shield::onDestroy()
{
    Object::onDestroy();

    auto* p = originSpawner();
    if (p && p->IsPlayer())
    {
        auto* player = static_cast<Player*>(p);
        player->set_shield(nullptr);
    }
    LOG_INFO("Shield:%s ondestroy", DebugString().c_str());

    return 0;
}
