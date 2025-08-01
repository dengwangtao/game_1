#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <memory>

#include "comm_def.h"


#include "scene_main.h"
#include "game.h"

int main(int, char**)
{
    // SDL_Log 设置
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    LOG_DEBUG("Debug message %s", "Hello, World!");

    auto& game = G_GAME;
    game.init();
    game.run();


    return 0;
}