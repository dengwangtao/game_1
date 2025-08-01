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

    s32 handleEvent(SDL_Event* event);
    s32 update();
    s32 render();

private:
    DEF_Property_default(bool, inited, false);
    DEF_Property(s32, is_running);
    DEF_Property(Scene*, current_scene); // 当前场景

    SDL_Window* window_ = nullptr;       // 窗口
    SDL_Renderer* renderer_ = nullptr;   // 渲染器

    DEF_Property_default(s32, window_width, 600);
    DEF_Property_default(s32, window_height, 800);
};














#endif // !GAME_H