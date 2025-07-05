#include <iostream>
#include <SDL.h>

#include "comm_def.h"

int main(int, char**)
{
    // SDL_Log 设置
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    LOG_DEBUG("Debug message %s", "Hello, World!");

    // SDL guard, 自动调用 SDL_Quit()
    auto sdl_mem = [](void*){
        LOG_DEBUG("SDL_Quit()");
        SDL_Quit();
    };
    s32 __add;
    std::unique_ptr<s32, decltype(sdl_mem)> sdl_guard(&__add, sdl_mem);


    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING)!= 0)
    {
        LOG_ERROR("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    constexpr s32 window_width  = 800;
    constexpr s32 window_height = 600;

    // 创建一个窗口
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window_guard(
        SDL_CreateWindow("Hello, World!", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            window_width, window_height, 
            SDL_WINDOW_SHOWN
        )
    , &SDL_DestroyWindow);

    // 创建一个渲染器
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_guard(
        SDL_CreateRenderer(window_guard.get(), -1, SDL_RENDERER_ACCELERATED)
        , &SDL_DestroyRenderer);


    // 渲染循环
    bool running = true;
    while (running)
    {
        // 处理事件
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        // 重置渲染器绘制颜色
        SDL_SetRenderDrawColor(renderer_guard.get(), 0, 0, 0, 255);
        // 清空窗口
        SDL_RenderClear(renderer_guard.get());


        // 绘制窗口
        SDL_SetRenderDrawColor(renderer_guard.get(), 255, 133, 255, 255);
        SDL_Rect rect{ 0, 0, window_width / 2, window_height / 2 };
        // SDL_RenderDrawRect(renderer_guard.get(), &rect);
        SDL_RenderFillRect(renderer_guard.get(), &rect);


        // 更新窗口
        SDL_RenderPresent(renderer_guard.get());
    }


    return 0;
}