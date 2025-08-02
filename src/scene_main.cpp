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
    if (player_)
    {
        delete player_;
        LOG_INFO("Player deleted");
    }
}

s32 SceneMain::init()
{
    player_ = new Player();
    player_->set_texture(
        IMG_LoadTexture(G_GAME.renderer(), "../assets/image/SpaceShip.png")
    );

    if (player_->texture() == nullptr)
    {
        LOG_ERROR("Failed to load texture");
        return -1;
    }

    s32 w, h;
    SDL_QueryTexture(player_->texture(), NULL, NULL, &w, &h);
    player_->set_height(h / 5);
    player_->set_width(w / 5);

    f32 px = (static_cast<f32>(G_GAME.window_width()) / 2 - static_cast<f32>(w) / 2);
    f32 py = (static_cast<f32>(G_GAME.window_height()) - h);
    player_->mutable_position()->x = px;
    player_->mutable_position()->y = py;

    LOG_INFO("Player created: %s", player_->DebugString().c_str());

    set_inited(true);
    return 0;
}
s32 SceneMain::update(s64 now_ms)
{
    keyboardControl();

    player_->update(now_ms);

    return 0;
}
s32 SceneMain::render()
{
    if (!inited())
    {
        LOG_ERROR("SceneMain not inited");
        return -1;
    }

    if (player_ == nullptr)
    {
        LOG_ERROR("Player is nullptr");
        return -1;
    }

    SDL_Rect rect = player_->GetRect();
    SDL_RenderCopy(G_GAME.renderer(), player_->texture(), NULL, &rect);
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

    return 0;
}
