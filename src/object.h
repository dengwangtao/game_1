#pragma once

#include "comm_def.h"
#include <SDL.h>
#include <string>

class Object
{
public:
    virtual ~Object();

    virtual s32 update(s64 now_ms);

    SDL_Rect GetRect() const;

    std::string DebugString() const;

    // 移动相关
    s32 UpdatePosition(s64 now_ms);

private:
    DEF_Property_default(s64, last_update_ms, 0); // 上次更新时间

    DEF_Property(SDL_Texture*, texture);// 贴图材质
    DEF_Property(SDL_FPoint, position); // 坐标
    DEF_Property(s32, width); // 宽度
    DEF_Property(s32, height); // 高度

    // 当前移动方向
    DEF_Property_default(s32, move_dir_x, 0); // 移动方向x
    DEF_Property_default(s32, move_dir_y, 0); // 移动方向y
    // 是否控制移动
    DEF_Property_default(bool, is_control_move, false); // 是否控制移动

    DEF_Property_default(f32, speed, 0.0f); // 当前速度
    DEF_Property_default(f32, max_speed, 400.0f); // 最大速度
    DEF_Property_default(f32, acceleration, 400.0f); // 加速度
    DEF_Property_default(f32, deceleration, 700.0f); // 减速度
};