#include "player.h"
#include "tools.h"
#include "game.h"
#include "bullet.h"
#include "scene_main.h"
#include <cmath>

Player::~Player()
{
}


s32 Player::UpdatePosition(s64 now_ms)
{
    f64 dt = (now_ms - last_update_ms()) / 1000.0f;
    if (last_update_ms() == 0)
    {
        dt = 0.0f;
    }

    // 更新速度
    if (is_control_move())
    {
        set_speed(speed() + acceleration() * dt);
        if (speed() > max_speed())
        {
            set_speed(max_speed());
        }
    }
    else
    {
        set_speed(speed() - deceleration() * dt);
        if (speed() < 0.0f)
        {
            set_speed(0.0f);
        }
    }


    // 若速度为0，直接返回，无需移动和包围盒更新
    if (speed() <= 0.0f)
    {
        return 0;
    }

    // 若方向为零向量，也无需移动
    f32 dir_x = move_dir_x();
    f32 dir_y = move_dir_y();
    if (dir_x == 0 && dir_y == 0)
    {
        return 0;
    }

    // 方向向量单位化
    f32 dir_fx = static_cast<f32>(dir_x);
    f32 dir_fy = static_cast<f32>(dir_y);
    f32 length_sq = dir_fx * dir_fx + dir_fy * dir_fy;

    if (length_sq > 0.0001f)
    {
        f32 inv_length = 1.0f / std::sqrt(length_sq);
        dir_fx *= inv_length;
        dir_fy *= inv_length;

        // 应用位移
        f32 dx = dir_fx * speed() * static_cast<f32>(dt);
        f32 dy = dir_fy * speed() * static_cast<f32>(dt);

        position_.x += dx;
        position_.y += dy;
    }

    // 限制在窗口边界内
    position_.x = Tools::clamp<f32>(position_.x, 0.0f, G_GAME.window_width() - width());
    position_.y = Tools::clamp<f32>(position_.y, 0.0f, G_GAME.window_height() - height());

    return 0;
}

s32 Player::init(const std::string& img_texture_path)
{
    Object::init(img_texture_path);
    
    // 设置方向
    set_move_dir_y(-1); // 向上

    // 设置size
    s32 w, h;
    SDL_QueryTexture(texture(), NULL, NULL, &w, &h);
    set_height(h / 5);
    set_width(w / 5);

    // 设置初始位置
    f32 px = (static_cast<f32>(G_GAME.window_width()) / 2 - static_cast<f32>(width()) / 2);
    f32 py = (static_cast<f32>(G_GAME.window_height()) - height());
    mutable_position()->x = px;
    mutable_position()->y = py;


    // 设置血量
    set_hp(10);
    set_attack(1);

    return 0;
}

bool Player::canShoot() const
{
    s64 now_ms = G_GAME.now_ms();

    if (now_ms >= shoot_last_time() + shoot_cooldown())
    {
        return true;
    }

    return false;
}

s32 Player::shoot()
{
    if (! canShoot())
    {
        return -1;
    }

    auto* cur_scene = scene();
    if (! cur_scene)
    {
        LOG_ERROR("cur_scene is null");
        return -1;
    }
    auto* bullet = cur_scene->addObject<Bullet>(cur_scene, this);
    if (! bullet)
    {
        LOG_ERROR("Failed to create bullet");
        return -1;
    }

    bullet->init("../assets/image/bullet-d.png");


    // 计算子弹位置
    auto born_pos = Tools::calculate_aligned_position(GetRect(), bullet->width(), bullet->height());

    // 设置位置
    bullet->mutable_position()->x = born_pos.x;
    bullet->mutable_position()->y = born_pos.y;
    
    // 设置方向, same as player
    bullet->set_move_dir_x(move_dir_x());
    bullet->set_move_dir_y(move_dir_y());

    // 继承攻击力
    bullet->set_attack(attack());

    set_shoot_last_time(G_GAME.now_ms());
    return 0;
}


s32 Player::onDestroy()
{
    auto* cur_scene = scene();
    if (! cur_scene)
    {
        LOG_ERROR("cur_scene is null");
        return -1;
    }

    auto* cur_main_scene = dynamic_cast<SceneMain*>(cur_scene);
    if (cur_main_scene)
    {
        cur_main_scene->set_player(nullptr);
        // TODO: game over
    }

    return 0;
}

s32 Player::onCollision(Object* other)
{
    if (! other)
    {
        return 0;
    }

    if (other->IsEnemy())
    {
        other->set_hp(other->hp() - attack());
        set_hp(hp() - other->attack());

        LOG_INFO("Player:%s collides with Enemy:%s", DebugString().c_str(), other->DebugString().c_str());
    }

    return 0;
}