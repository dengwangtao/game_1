#include "scene_main.h"

#include <SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "player.h"
#include "animation.h"
#include <SDL_mixer.h>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <SDL_ttf.h>
#include "resource_mgr.h"

SceneMain::SceneMain()
{

}

SceneMain::~SceneMain()
{
}

s32 SceneMain::init()
{
    // load bgm
    bgm_ = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm_ == nullptr)
    {
        LOG_ERROR("Mix_LoadMUS error: %s", Mix_GetError());
        throw std::runtime_error("Mix_LoadMUS error");
        return -1;
    }
    // 循环播放bgm
    Mix_PlayMusic(bgm_, -1);
    // 设置音量
    Mix_VolumeMusic(5);
    // 设置所有音效的音量
    Mix_Volume(-1, 5);


    enemy_spawner_.init();
    item_spawner_.init();

    player_ = addObject<Player>();
    player_->init("../assets/image/SpaceShip.png");
    
    LOG_INFO("Player created: %s", player_->DebugString().c_str());

    set_inited(true);
    return 0;
}
s32 SceneMain::update(s64 now_ms)
{
    Scene::update(now_ms);

    keyboardControl();

    enemy_spawner_.update(now_ms);
    item_spawner_.update(now_ms);

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

    renderScore();

    for (auto& obj : objects_)
    {
        obj.second->render();
    }

    for (auto& ani : animations_)
    {
        ani->render();
    }

    return 0;
}
s32 SceneMain::clean()
{
    if (bgm())
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm_);
        set_bgm(nullptr);
    }

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

    if (player_ != nullptr)
    {
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

        
    }


    return 0;
}

s32 SceneMain::renderScore()
{
    auto* font = G_RESOURCE_MGR.loadResource<TTF_Font>("../assets/font/VonwaonBitmap-16px.ttf", 16);

    if (!font)
    {
        LOG_ERROR("Failed to load font");
        return -1;
    }
    // 渲染每个玩家的分数
    int y_offset = 0;

    foreachObject<Player>([&](Player& player) -> s32 {
        // 拼接玩家信息
        std::string text = std::to_string(player.guid()) + " score: " + std::to_string(player.score());


        // 创建每行的 surface
        std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface(TTF_RenderUTF8_Solid(font, text.c_str(), {255, 255, 255, 255}), &SDL_FreeSurface);
        if (! surface)
        {
            LOG_ERROR("Failed to render text: %s", TTF_GetError());
            return -1;
        }

        // 创建每行的 texture
        std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture(SDL_CreateTextureFromSurface(G_GAME.renderer(), surface.get()), &SDL_DestroyTexture);
        if (!texture)
        {
            LOG_ERROR("Failed to create texture: %s", SDL_GetError());
            return -1;
        }

        // 计算位置
        SDL_Rect dst_rect = {0, y_offset, surface->w, surface->h};
        y_offset += surface->h;  // 增加 y_offset 以便下行文本渲染到下方

        // 渲染当前行
        s32 ret = SDL_RenderCopy(G_GAME.renderer(), texture.get(), nullptr, &dst_rect);
        if (ret)
        {
            LOG_ERROR("Failed to render texture: %s", SDL_GetError());
            return -1;
        }

        return 0;
    });

    return 0;
}