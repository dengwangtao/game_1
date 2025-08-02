#include "scene_main.h"

#include <SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "player.h"


SceneMain::SceneMain()
{

}

SceneMain::~SceneMain()
{
}

s32 SceneMain::init()
{
    player_ = addObject<Player>(this);
    player_->init("../assets/image/SpaceShip.png");
    
    LOG_INFO("Player created: %s", player_->DebugString().c_str());

    set_inited(true);
    return 0;
}
s32 SceneMain::update(s64 now_ms)
{
    Scene::update(now_ms);

    keyboardControl();

    // 遍历所有对象, update
    for (auto& obj : objects_)
    {
        obj.second->update(now_ms);
    }

    return 0;
}
s32 SceneMain::render()
{
    if (!inited())
    {
        LOG_ERROR("SceneMain not inited");
        return -1;
    }

    for (auto& obj : objects_)
    {
        obj.second->render();
    }

    return 0;
}
s32 SceneMain::clean()
{    
    return 0;
}
s32 SceneMain::handleEvent(SDL_Event*)
{

    return 0;
}

s32 SceneMain::keyboardControl()
{
    s32 keynums = 0;
    auto key_state = SDL_GetKeyboardState(&keynums);

    // 移动相关
    s32 dir_x = 0;
    s32 dir_y = 0;
    if (key_state[SDL_SCANCODE_W] && key_state[SDL_SCANCODE_S])
    {
        dir_y = 0;
    }
    else if (key_state[SDL_SCANCODE_W])
    {
        dir_y = -1;
    }
    else if (key_state[SDL_SCANCODE_S])
    {
        dir_y = 1;
    }
    
    if (key_state[SDL_SCANCODE_A] && key_state[SDL_SCANCODE_D])
    {
        dir_x = 0;
    }
    else if (key_state[SDL_SCANCODE_A])
    {
        dir_x = -1;
    }
    else if (key_state[SDL_SCANCODE_D])
    {
        dir_x = 1;
    }

    if (dir_x != 0 || dir_y != 0)
    {
        player_->set_move_dir_x(dir_x);
        player_->set_move_dir_y(dir_y);
        player_->set_is_control_move(true);
    }
    else
    {
        player_->set_is_control_move(false); // 停止移动, 进入减速状态
    }



    // 发射相关
    if (key_state[SDL_SCANCODE_SPACE])
    {
        if (player_->canShoot())
        {
            player_->shoot();
        }
    }



    return 0;
}
