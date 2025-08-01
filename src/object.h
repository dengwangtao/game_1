#pragma once

#include "comm_def.h"
#include <SDL.h>
#include <string>
#include "object_type.h"

class Scene;

class Object
{
    const static ObjectType ObjType = ObjectType::NONE;
public:
    Object(ObjectType objtype, Scene* scene, Object* spawner=nullptr);

    virtual ~Object();

    virtual s32 init(const std::string& img_texture_path="");
    virtual s32 update(s64 now_ms);
    virtual s32 render();
    virtual s32 clean() { return 0; }
    virtual s32 makeTexture(const std::string& file_path);

    bool IsPlayer() const { return obj_type() == ObjectType::PLAYER; }
    bool IsBullet() const { return obj_type() == ObjectType::BULLET; }
    bool IsEnemy() const { return obj_type() == ObjectType::ENEMY; }


    SDL_FRect GetRect() const;

    std::string DebugString() const;

    // 移动相关
    virtual s32 UpdatePosition(s64 now_ms);

protected:
    DEF_Property(Scene*, scene); // 所属场景
    DEF_Property(u64, guid); // 全局唯一标识符
    DEF_Property(ObjectType, obj_type); // 对象类型

    DEF_Property_default(s64, last_update_ms, 0); // 上次更新时间

    DEF_Property(SDL_Texture*, texture);// 贴图材质
    DEF_Property(SDL_FPoint, position); // 坐标
    DEF_Property(s32, width); // 宽度
    DEF_Property(s32, height); // 高度

    DEF_Property(Object*, spawner); // 发射者
};