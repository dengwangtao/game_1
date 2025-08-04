#pragma once

#include "object.h"


class Enemy : public Object
{
    const static ObjectType ObjType = ObjectType::ENEMY;
public:
    using Object::Object;

    s32 init(const std::string& img_texture_path="") override;
    s32 update(s64 now_ms) override;

    s32 UpdatePosition(s64 now_ms) override;

protected:
    DEF_Property_default(f32, speed, 200.0f); // Enemy speed
    // DEF_Property_default(f32, damage, 10.0f); // Enemy damage

    // 当前移动方向
    DEF_Property_default(s32, move_dir_x, 0); // 移动方向x
    DEF_Property_default(s32, move_dir_y, 1); // 移动方向y
};