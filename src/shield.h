#pragma once

#include "object.h"


class Shield : public Object
{
    const static ObjectType ObjType = ObjectType::SHIELD;
public:
    using Object::Object;

    s32 init(const std::string& img_texture_path) override;
    s32 update(s64 now_ms) override;

    s32 onCollision(Object* other) override;

    s32 UpdatePosition(s64 now_ms) override;

    s32 onDestroy() override;

protected:

};