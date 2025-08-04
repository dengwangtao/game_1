#pragma once

#include "comm_def.h"
#include <SDL.h>
#include <string>

class Scene;

class Animation
{
public:
    Animation(Scene* scene);

    virtual ~Animation();

    virtual s32 init(const std::string& img_texture_path="");
    virtual s32 update(s64 now_ms);
    virtual s32 render();
    virtual s32 makeTexture(const std::string& file_path);


    SDL_FRect GetRect() const;
    SDL_FPoint GetCenter() const;

    std::string DebugString() const;


protected:
    DEF_Property(Scene*, scene); // 所属场景

    DEF_Property_default(s64, last_update_ms, 0); // 上次更新时间

    DEF_Property(SDL_Texture*, texture);// 贴图材质
    DEF_Property(SDL_FPoint, position); // 坐标
    DEF_Property(s32, width); // 宽度
    DEF_Property(s32, height); // 高度
};