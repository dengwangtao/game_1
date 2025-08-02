#pragma once
#ifndef GAME_H
#define GAME_H

#include "comm_def.h"
#include "scene.h"
#include "singleton.h"

struct SDL_Window;
struct SDL_Renderer;

class Game : public Singleton<Game>
{

public:
	Game();
	~Game();

    s32 run();
    s32 init();
    s32 clean();
    s32 changeScene(Scene* scene); // 切换场景

    s32 handleEvent(SDL_Event* event);
    s32 update(s64 now_ms);
    s32 render();

private:
    DEF_Property_default(bool, inited, false);
    DEF_Property(s32, is_running);
    DEF_Property(Scene*, current_scene); // 当前场景

    DEF_Property(SDL_Window*, window); // 窗口
    DEF_Property(SDL_Renderer*, renderer); // 渲染器

    DEF_Property_default(s32, window_width, 600);
    DEF_Property_default(s32, window_height, 800);

    DEF_Property_default(s64, now_ms, 0); // 当前时间
};


#define G_GAME Game::Instance()











#endif // !GAME_H