#pragma once

#include "object.h"
#include "item_types.h"




class Item : public Object
{
    const static ObjectType ObjType = ObjectType::BULLET;
public:
    using Object::Object;

    s32 init(const std::string& img_texture_path) override;
    s32 update(s64 now_ms) override;

    s32 onCollision(Object* other) override;

    s32 UpdatePosition(s64 now_ms) override;

    s32 initItem(ItemType type);

    s32 applyEffect(Object* other); // apply 道具效果

protected:
    DEF_Property_default(f32, speed, 150.0f); // Bullet speed
    // DEF_Property_default(f32, damage, 10.0f); // Bullet damage

    DEF_Property_default(s32, rebound_count, 3); // 道具反弹次数

    DEF_Property_default(ItemType, type, ITEM_NONE); // 道具类型
};