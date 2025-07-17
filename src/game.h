#pragma once
#ifndef GAME_H
#define GAME_H

#include "comm_def.h"
#include "scene.h"
#include <SDL.h>


class Game
{

public:
	Game();
	~Game();

    s32 run();
    s32 init();
    s32 clean();
    s32 changeScene(Scene* scene); // 切换场景

private:
    DEF_Property(s32, is_running);
    DEF_Property(Scene*, current_scene); // 当前场景
};














#endif // !GAME_H