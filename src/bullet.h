#pragma once

#include "object.h"


class Bullet : public Object
{
    const static ObjectType ObjType = ObjectType::BULLET;
public:
    using Object::Object;

    s32 init(const std::string& img_texture_path="") override;
    s32 update(s64 now_ms) override;

    s32 UpdatePosition(s64 now_ms) override;

protected:
    DEF_Property_default(f32, speed, 200.0f); // Bullet speed
    // DEF_Property_default(f32, damage, 10.0f); // Bullet damage

    // 当前移动方向
    DEF_Property_default(s32, move_dir_x, 0); // 移动方向x
    DEF_Property_default(s32, move_dir_y, 0); // 移动方向y
};