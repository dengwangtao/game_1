#include "enemy.h"
#include "game.h"
#include "tools.h"
#include "bullet.h"
#include "scene.h"
#include "animation.h"
#include <cmath>

s32 Enemy::init(const std::string& img_texture_path)
{
    Object::init(img_texture_path);

    set_move_dir_x(0);
    set_move_dir_y(1);

    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h / 2);
    set_width(w / 2);
    LOG_INFO("Enemy init: %s", DebugString().c_str());

    // 设置血量
    set_hp(3);
    set_attack(1);

    return 0;
}

s32 Enemy::update(s64 now_ms)
{
    Object::update(now_ms);

    shoot();

    return 0;
}

s32 Enemy::UpdatePosition(s64 now_ms)
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
        LOG_INFO("Enemy out of game"); // 销毁
        scene()->markRemoveObject(this);
    }
    return 0;
}

s32 Enemy::shoot()
{   
    if (G_GAME.now_ms() < shoot_last_time() + shoot_cooldown())
    {
        return 0;
    }

    bool is_shoot = Tools::random(0, 1);
    if (! is_shoot)
    {
        return 0;
    }

    auto* cur_scene = scene();
    if (! cur_scene)
    {
        LOG_ERROR("cur_scene is null");
        return -1;
    }

    if (cur_scene->playerCount() <= 0)
    {
        return 0;
    }

    // 随机一个玩家
    s32 rdm_idx = Tools::random(0, cur_scene->playerCount() - 1);

    // 计算与玩家的角度
    Player* p = nullptr;
    cur_scene->foreachObject<Player>([&](Player& player) {
        p = &player;
        if (rdm_idx == 0)
        {
            return 1;
        }
        rdm_idx--;
        return 0;
    });

    if (!p)
    {
        LOG_ERROR("No player");
        return -2;
    }

    // 计算角度
    f64 angle = atan2(p->GetCenter().y - GetCenter().y, p->GetCenter().x - GetCenter().x);
    // 发射范围 左右45度
    if (angle > M_PI / 4 * 3 || angle < M_PI / 4)
    {
        return 0;
    }


    auto* bullet = cur_scene->addObject<Bullet>(cur_scene, this);
    if (! bullet)
    {
        LOG_ERROR("Failed to create bullet");
        return -1;
    }

    bullet->init("../assets/image/bullet-1.png");


    // 计算位置
    auto born_pos = Tools::calculate_aligned_position(GetRect(), bullet->width(), bullet->height());

    // 设置位置
    bullet->mutable_position()->x = born_pos.x;
    bullet->mutable_position()->y = born_pos.y;

    // 设置方向
    bullet->set_move_dir_x(cos(angle));
    bullet->set_move_dir_y(sin(angle));
    LOG_INFO("Enemy shoot: %s", bullet->DebugString().c_str());


    // 继承攻击力
    bullet->set_attack(attack());

    set_shoot_last_time(G_GAME.now_ms());


    return 0;
}



s32 Enemy::onDestroy()
{
    auto* cur_scene = scene();
    if (! cur_scene)
    {
        LOG_ERROR("cur_scene is null");
        return -1;
    }

    auto* ani = cur_scene->addAnimation<Animation>(cur_scene);
    if (! ani)
    {
        LOG_ERROR("Failed to create animation");
        return -2;
    }

    ani->init("../assets/effect/explosion.png");

    // 设置位置
    auto born_pos = Tools::calculate_aligned_position(GetRect(), ani->width(), ani->height());
    ani->mutable_position()->x = born_pos.x;
    ani->mutable_position()->y = born_pos.y;

    ani->play(800);

    return 0;
}