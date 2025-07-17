#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "comm_def.h"

#include <SDL.h>


class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

    virtual s32 init() = 0;
    virtual s32 update() = 0;
    virtual s32 render() = 0;
    virtual s32 clean() = 0;
    virtual s32 handleEvent(SDL_Event* event) = 0;
};





#endif // !SCENE_H