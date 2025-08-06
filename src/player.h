#pragma once

#include "object.h"

class Shield;

class Player final : public Object
{
    const static ObjectType ObjType = ObjectType::PLAYER;
public:
    ~Player() override;

    using Object::Object;

    s32 init(const std::string& img_texture_path) override;
    s32 onDestroy() override;

    s32 onCollision(Object* other) override;

    bool canShoot() const;
    s32 shoot();


    s32 UpdatePosition(s64 now_ms) override;

    s32 onKill(Object* other) override;
    s32 onGetItem(Item* item) override;

    s32 addShield();

    s32 applyTimeItem();

private:
    // 是否控制移动
    DEF_Property_default(bool, is_control_move, false); // 是否控制移动

    DEF_Property_default(f32, speed, 0.0f); // 当前速度
    DEF_Property_default(f32, max_speed, 400.0f); // 最大速度
    DEF_Property_default(f32, acceleration, 400.0f); // 加速度
    DEF_Property_default(f32, deceleration, 700.0f); // 减速度


    // 发射相关
    DEF_Property_default(s32, shoot_cooldown, 200); // 射击冷却时间, 单位: ms
    DEF_Property_default(s64, shoot_last_time, 0); // 上次射击时间

    DEF_Property_default(Shield*, shield, nullptr); // 盾牌

    DEF_Property_default(s32, score, 0); // 分数

};