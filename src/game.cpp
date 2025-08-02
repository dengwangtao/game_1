#include "game.h"
#include "scene_main.h"

#include <chrono>
#include <SDL.h>
#include <SDL_image.h>

Game::Game()
{
}

Game::~Game()
{
    clean();
}

s32 Game::run()
{
    if (! inited())
    {
        LOG_ERROR("game not inited");
        return -1;
    }

    LOG_INFO("game run");

    set_is_running(true);
    
    while (is_running())
    {
        // 获取当前时间 ns
        s64 now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        set_frame_interval(1000.0f / frame_rate());
        
        auto frame_start_time = now_ms;

        SDL_Event event;

        handleEvent(&event);

        if (! current_scene())
        {
            LOG_DEBUG("current scene is null");
            continue;
        }
        
        update(now_ms);

        render();
        
        // 计算下一帧需要的时间
        s64 frame_end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        s64 frame_time = frame_end_time - frame_start_time;

        // 控制帧率
        if (frame_time < frame_interval())
        {
            SDL_Delay(static_cast<u32>(frame_interval() - frame_time));
        }
        else
        {
            // 掉帧
            LOG_WARN("frame time %lld > frame interval %lld", frame_time, frame_interval());
        }
    }

    return 0;
}

s32 Game::init()
{
    // SDL初始化
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LOG_ERROR("SDL_Init error: %s", SDL_GetError());
        set_is_running(false);
    }

    // 初始化SDL_image
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        LOG_ERROR("IMG_Init error: %s", IMG_GetError());
        set_is_running(false);
    }
    
    // 创建窗口
    window_ = SDL_CreateWindow("DWT Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            window_width(), window_height(), SDL_WINDOW_SHOWN);
    if (! window_)
    {
        LOG_ERROR("SDL_CreateWindow error: %s", SDL_GetError());
        set_is_running(false);
    }

    // 创建渲染器
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (! renderer_)
    {
        LOG_ERROR("SDL_CreateRenderer error: %s", SDL_GetError());
        set_is_running(false);
    }

    // 创建初始场景
    {
        SceneMain* scene = new SceneMain();
        changeScene(scene);
    }

    set_inited(true);
    return 0;
}

s32 Game::clean()
{
    if (! inited())
    {
        return 0;
    }

    // 清理当前场景
    if (current_scene())
    {
        current_scene()->clean();
        delete current_scene();
        set_current_scene(nullptr);
        LOG_DEBUG("clean current scene");
    }


    // 清理窗口
    if (window_)
    {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        LOG_DEBUG("SDL_DestroyWindow");
    }

    // 清理渲染器
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
        LOG_DEBUG("SDL_DestroyRenderer");
    }

    // 清理SDL_image
    IMG_Quit();
    LOG_DEBUG("IMG_Quit");

    // 退出SDL
    SDL_Quit();
    LOG_DEBUG("SDL_Quit");

    return 0;
}

s32 Game::changeScene(Scene *scene)
{
    if (! scene)
    {
        LOG_ERROR("scene is null");
        return -1;
    }

    LOG_INFO("change scene to %p", (void*)scene);

    if (current_scene())
    {
        current_scene()->clean();
        delete current_scene();
    }

    set_current_scene(scene);
    current_scene()->init();
    
    return 0;
}

s32 Game::handleEvent(SDL_Event* event)
{
    if (! event)
    {
        LOG_ERROR("event is null");
        return -1;
    }

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            LOG_DEBUG("SDL_QUIT");
            set_is_running(false);
        }
        if (current_scene())
        {
            current_scene()->handleEvent(event);
        }
    }
    return 0;
}
s32 Game::update(s64 now_ms)
{
    if (current_scene())
    {
        current_scene()->update(now_ms);
    }

    return 0;
}
s32 Game::render()
{
    // 三步: 1. 清除屏幕 2. 绘制场景 3. 更新屏幕
    SDL_RenderClear(renderer_);

    if (current_scene())
    {
        current_scene()->render();
    }

    SDL_RenderPresent(renderer_);

    return 0;
}
