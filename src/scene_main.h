#pragma once
#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H


#include "scene.h"
#include "enemy_spawner.h"
#include "item_spawner.h"

class Player;

class SceneMain : public Scene
{

public:
    SceneMain();
    ~SceneMain() override;

    s32 init() override;
    s32 update(s64 now_ms) override;
    s32 render() override;
    s32 clean() override;
    s32 handleEvent(SDL_Event* event) override;

    s32 keyboardControl();

private:
    DEF_Property_default(bool, inited, false);
    DEF_Property_default(Player*, player, nullptr);

    EnemySpawner enemy_spawner_;
    ItemSpawner item_spawner_;
};











#endif // SCENE_MAIN_H