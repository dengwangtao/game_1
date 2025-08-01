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
    player_->set_height(h);
    player_->set_width(w);

    f32 px = (static_cast<f32>(G_GAME.window_width()) / 2 - static_cast<f32>(w) / 2);
    f32 py = (static_cast<f32>(G_GAME.window_height()) - h);
    player_->mutable_position()->x = px;
    player_->mutable_position()->y = py;

    LOG_INFO("Player created: %s", player_->DebugString().c_str());

    set_inited(true);
    return 0;
}
s32 SceneMain::update()
{

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
s32 SceneMain::handleEvent(SDL_Event* event)
{

    return 0;
}