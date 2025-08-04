#include "item.h"
#include "tools.h"
#include "game.h"
#include "tools.h"

#include <cmath>

s32 Item::init(const std::string& img_texture_path)
{
    Object::init(img_texture_path);

    // 随机一个方向
    // pi/4~3pi/4
    f32 angle = Tools::random(M_PI / 4, 3 * M_PI / 4);
    set_move_dir_x(cos(angle));
    set_move_dir_y(sin(angle));


    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h / 2.0f);
    set_width(w / 2.0f);
    LOG_INFO("Item init: %s", DebugString().c_str());
    
    return 0;
}

s32 Item::update(s64 now_ms)
{
    Object::update(now_ms);

    return 0;
}

s32 Item::UpdatePosition(s64 now_ms)
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
    auto game_rect = SDL_FRect{0, -static_cast<f32>(height()), static_cast<f32>(G_GAME.window_width()), static_cast<f32>(G_GAME.window_height() + height())};
    SDL_FRect result;
    bool is_intersect = SDL_IntersectFRect(&this_rect, &game_rect, &result);
    if (! is_intersect)
    {
        LOG_INFO("Bullet out of game"); // 销毁
        scene()->markRemoveObject(this);
    }

    if (rebound_count() > 0)
    {
        // 左右边界反弹检查
        f32 left = 0.0f;
        f32 right = static_cast<f32>(G_GAME.window_width());
        f32 radius = width() / 2.0f;

        bool bounced = false;
        if (position_.x - radius < left)
        {
            position_.x = left + radius;
            set_move_dir_x(-move_dir_x());
            bounced = true;
        }
        if (position_.x + radius > right)
        {
            position_.x = right - radius;
            set_move_dir_x(-move_dir_x());
            bounced = true;
        }

        if (bounced)
        {
            set_rebound_count(rebound_count() - 1);
        }
    }
    

    return 0;
}

s32 Item::initItem(ItemType type)
{
    auto name = ItemTypes::GetItemName(type);
    auto texture_path = ItemTypes::GetItemFile(type);
    s32 ret = init(std::string(texture_path));
    set_type(type);

    LOG_INFO("Item init: %s", DebugString().c_str());
    return ret;
}

s32 Item::applyEffect(Object *other)
{
    switch (type())
    {
    case ItemType::ITEM_HEALTH:
    {
        // 给player加血
        other->set_hp(other->hp() + 1);
        break;
    }
    case ItemType::ITEM_SHIELD:
    {
        // TODO: 添加盾牌效果
        break;
    }
    case ItemType::ITEM_TIME:
    {
        // TODO: 
        break;
    }
    default:
    {
        break;
    }
    }
    return 0;
}

s32 Item::onCollision(Object* other)
{
    if (! other)
    {
        return 0;
    }
    if (! other->IsPlayer())
    {
        return 0;
    }

    // 只处理碰撞到player
    LOG_INFO("Item:%s collided with player:%s", DebugString().c_str(), other->DebugString().c_str());
    applyEffect(other);


    scene()->markRemoveObject(this);

    return 0;
}