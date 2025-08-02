#pragma once

#include "object.h"

class Player final : public Object
{

public:
    ~Player() override;

    using Object::Object;

    bool canShoot() const;
    s32 shoot();


    s32 UpdatePosition(s64 now_ms) override;

private:
    // 当前移动方向
    DEF_Property_default(s32, move_dir_x, 0); // 移动方向x
    DEF_Property_default(s32, move_dir_y, 0); // 移动方向y
    // 是否控制移动
    DEF_Property_default(bool, is_control_move, false); // 是否控制移动

    DEF_Property_default(f32, speed, 0.0f); // 当前速度
    DEF_Property_default(f32, max_speed, 400.0f); // 最大速度
    DEF_Property_default(f32, acceleration, 400.0f); // 加速度
    DEF_Property_default(f32, deceleration, 700.0f); // 减速度


    // 发射相关
    DEF_Property_default(s32, shoot_cooldown, 500); // 射击冷却时间, 单位: ms
    DEF_Property_default(s64, shoot_last_time, 0); // 上次射击时间
};