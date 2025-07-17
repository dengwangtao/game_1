#include "game.h"

Game::Game()
{
}

Game::~Game()
{

}

s32 Game::run()
{
    set_is_running(true);
    
    while (is_running())
    {
        if (! current_scene())
        {
            LOG_DEBUG("current scene is null");
            continue;
        }

        SDL_Event event;
        
        current_scene()->handleEvent(&event);
        current_scene()->update();
        current_scene()->render();

    }

    return 0;
}

s32 Game::init()
{

    return 0;
}

s32 Game::clean()
{

    return 0;
}

s32 Game::changeScene(Scene *scene)
{
    if (! scene)
    {
        LOG_ERROR("scene is null");
        return -1;
    }

    if (current_scene())
    {
        current_scene()->clean();
        delete current_scene();
    }

    set_current_scene(scene);
    current_scene()->init();
    
    return 0;
}
