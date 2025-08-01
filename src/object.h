#pragma once

#include "comm_def.h"
#include <SDL.h>
#include <string>

class Object
{
public:
    virtual ~Object();

    SDL_Rect GetRect() const;

    std::string DebugString() const;

private:

    DEF_Property(SDL_Texture*, texture);// 贴图材质
    DEF_Property(SDL_FPoint, position); // 坐标
    DEF_Property(s32, width); // 宽度
    DEF_Property(s32, height); // 高度

};