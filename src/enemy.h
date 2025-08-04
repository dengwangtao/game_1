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

    s32 shoot();


protected:
    DEF_Property_default(f32, speed, 200.0f); // Enemy speed
    // DEF_Property_default(f32, damage, 10.0f); // Enemy damage

    DEF_Property_default(s64, shoot_cooldown, 2000); // 射击冷却时间
    DEF_Property_default(s64, shoot_last_time, 0); // 上次射击时间
};