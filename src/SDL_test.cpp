#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <memory>

#include "comm_def.h"


#if 0

int main(int, char**)
{
    // SDL_Log 设置
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);

    LOG_DEBUG("Debug message %s", "Hello, World!");


#pragma region SDL initialization
    // SDL 初始化
    if (SDL_Init(SDL_INIT_EVERYTHING)!= 0)
    {
        LOG_ERROR("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }
    // SDL guard, 自动调用 SDL_Quit()
    auto sdl_mem = [](void*){
        LOG_DEBUG("SDL_Quit()");
        SDL_Quit();
    };
    s32 __sdl_quit;
    std::unique_ptr<s32, decltype(sdl_mem)> sdl_guard(&__sdl_quit, sdl_mem);
#pragma endregion

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


#pragma region init SDL_image
    // 初始化SDL_image
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG))
    {
        LOG_ERROR("IMG_Init Error: %s", IMG_GetError());
        return 1;
    }
    // SDL_image guard, 自动调用 IMG_Quit()
    auto img_mem = [](void*){
        LOG_DEBUG("IMG_Quit()");
        IMG_Quit();
    };
    s32 __img_quit;
    std::unique_ptr<s32, decltype(img_mem)> sdl_image_guard(&__img_quit, img_mem);
#pragma endregion

    // 加载图片
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture_guard(
        IMG_LoadTexture(renderer_guard.get(), "../assets/image/bg.png")
       , &SDL_DestroyTexture);

#pragma region SDL_mixer initialization
    // SDL_mixer 初始化
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
    {
        LOG_ERROR("Mix_OpenAudio Error: %s", Mix_GetError());
        return 1;
    }
    // SDL_mixer guard, 自动调用 Mix_CloseAudio() 和 Mix_Quit()
    auto mix_mem = [](void*){
        LOG_DEBUG("Mix_CloseAudio()");
        Mix_CloseAudio();
        LOG_DEBUG("Mix_Quit()");
        Mix_Quit();
    };
    s32 __mix_quit;
    std::unique_ptr<s32, decltype(mix_mem)> sdl_mixer_guard(&__mix_quit, mix_mem);
#pragma endregion

    // 加载音效
    std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> music_guard(
        Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg")
        , &Mix_FreeMusic);
    
    if (music_guard.get() == nullptr)
    {
        LOG_ERROR("Mix_LoadMUS Error: %s", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(music_guard.get(), -1); // 无限循环播放音乐
    

#pragma region SDL_ttf initialization
    // 初始化SDL_ttf
    if (TTF_Init() != 0)
    {
        LOG_ERROR("TTF_Init Error: %s", TTF_GetError());
        return 1;
    }
    // SDL_ttf guard, 自动调用 TTF_Quit()
    auto ttf_mem = [](void*){
        LOG_DEBUG("TTF_Quit()");
        TTF_Quit();
    };
    s32 __ttf_quit;
    std::unique_ptr<s32, decltype(ttf_mem)> sdl_ttf_guard(&__ttf_quit, ttf_mem);
#pragma endregion

    // 加载字体
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font_guard(
        TTF_OpenFont("../assets/font/VonwaonBitmap-12px.ttf", 24)
        , &TTF_CloseFont);

    if (font_guard.get() == nullptr)
    {
        LOG_ERROR("TTF_OpenFont Error: %s", TTF_GetError());
        return 1;
    }

    // 创建文本
    SDL_Color color{ 255, 255 };
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> text_surface_guard(
        TTF_RenderUTF8_Solid(font_guard.get(), "Hello, World! 中文", color)
        , &SDL_FreeSurface);
    
    // 转换为纹理
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> text_texture_guard(
        SDL_CreateTextureFromSurface(renderer_guard.get(), text_surface_guard.get())
       , &SDL_DestroyTexture);




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


        // 绘制图片
        SDL_Rect texture_rect{ 100, 100, 500, 500 };
        SDL_RenderCopy(renderer_guard.get(), texture_guard.get(), nullptr, &texture_rect);

        // 绘制文本
        SDL_Rect text_rect{ 100, 100, text_surface_guard->w, text_surface_guard->h };
        SDL_RenderCopy(renderer_guard.get(), text_texture_guard.get(), nullptr, &text_rect);

        // 更新窗口
        SDL_RenderPresent(renderer_guard.get());
    }


    return 0;
}

#endif