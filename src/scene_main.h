#pragma once
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H


#include "scene.h"



class SceneMain : public Scene
{

public:
    SceneMain();
    ~SceneMain() override;

    s32 init() override;
    s32 update() override;
    s32 render() override;
    s32 clean() override;
    s32 handleEvent(SDL_Event* event) override;

private:
};











#endif // SCENE_MAIN_H