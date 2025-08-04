#include "object.h"
#include "tools.h"
#include "game.h"
#include "guid_gen.h"
#include "scene.h"
#include "texture_mgr.h"
#include "animation.h"
#include <SDL.h>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>


Object::Object(ObjectType objtype, Scene *scene, Object* spawner)
    : obj_type_(objtype)
    , scene_(scene)
    , spawner_(spawner)
{
    set_guid(GUIDGen::GenerateGUID(obj_type()));
    if (! spawner_)
    {
        set_spawner(this);
    }

    LOG_INFO("Create object: guid=%s, spawner=%s, objtype=%d",
        GUIDGen::ParseGUID(guid()).c_str(),
        spawner_->DebugString().c_str(),
        static_cast<int>(obj_type_));
}

Object::~Object()
{
    LOG_INFO("Destroy object: guid=%s", GUIDGen::ParseGUID(guid()).c_str());
}

s32 Object::init(const std::string &img_texture_path)
{
    s32 ret = makeTexture(img_texture_path);
    if (ret)
    {
        LOG_ERROR("Failed to load texture");
        return ret;
    }

    return 0;
}

s32 Object::update(s64 now_ms)
{
    if (hp() <= 0)
    {
        scene()->markRemoveObject(this);
    }

    UpdatePosition(now_ms);


    set_last_update_ms(now_ms);
    return 0;
}

s32 Object::render()
{
    SDL_FRect rect = GetRect();
    s32 ret = 0;
    // ret = SDL_RenderCopyF(G_GAME.renderer(), texture(), NULL, &rect);

    if (IsBullet())
    {
        // 旋转
        f32 dirx = move_dir_x();
        f32 diry = move_dir_y();
        f32 angle = atan2(diry, dirx) * 180.0f / M_PI - 90.0f;
        ret = SDL_RenderCopyExF(G_GAME.renderer(), texture(), NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
    else if (IsPlayer())
    {
        // 垂直翻转
        ret = SDL_RenderCopyExF(G_GAME.renderer(), texture(), NULL, &rect, 0, NULL, SDL_FLIP_VERTICAL);
    }
    else if (IsEnemy())
    {
        ret = SDL_RenderCopyExF(G_GAME.renderer(), texture(), NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        ret = SDL_RenderCopyF(G_GAME.renderer(), texture(), NULL, &rect);
    }

    renderHP();


    if (ret)
    {
        LOG_ERROR("Failed to render object: %s", SDL_GetError());
        return ret;
    }
    return 0;
}

s32 Object::makeTexture(const std::string &file_path)
{
    set_texture(
        G_TEXTURE_MGR.LoadTexture(file_path)
    );

    if (texture() == nullptr)
    {
        LOG_ERROR("Failed to load texture");
        return -1;
    }
    return 0;
}
Object *Object::originSpawner(int depth) const
{
    // 防止死递归
    if (depth > 10)
    {
        LOG_ERROR("originSpawner depth too deep");
        return nullptr;
    }
    if (spawner() == nullptr || spawner() == this)
    {
        return spawner();
    }
    return spawner()->originSpawner(depth + 1);
}
SDL_FRect Object::GetRect() const
{
    return SDL_FRect{
        static_cast<f32>(position().x),
        static_cast<f32>(position().y),
        static_cast<f32>(width()),
        static_cast<f32>(height())
    };
}

SDL_FPoint Object::GetCenter() const
{
    return Tools::get_rect_center(GetRect());
}

std::string Object::DebugString() const
{
    std::ostringstream oss;
    oss << "Object: " << this << " pos: " << position().x << "," << position().y << " size: " << width() << "," << height();
    return oss.str();
}

s32 Object::UpdatePosition(s64 now_ms)
{
    return 0;
}

s32 Object::renderHP()
{
    static const s32 heart_w = 16;
    static const s32 heart_h = 16;
    static const s32 gap = 2;
    static const s32 hearts_per_line = 4;

    s32 ret = 0;
    if (IsPlayer() || IsEnemy())
    {
        auto* heart_texture = G_TEXTURE_MGR.LoadTexture("../assets/image/Health UI Black.png");
        if (heart_texture == nullptr)
        {
            LOG_ERROR("Failed to load texture");
        }
        else
        {
            s32 cur_hp = hp();

            s32 lines = (cur_hp + hearts_per_line - 1) / hearts_per_line;

            s32 total_w = std::min(cur_hp, hearts_per_line) * heart_w + (std::min(cur_hp, hearts_per_line) - 1) * gap;
            s32 total_h = lines * heart_h + (lines - 1) * gap;

            auto draw_rect = SDL_FRect {
                position().x,
                position().y + height(), // 渲染在对象下方
                static_cast<f32>(width()),
                static_cast<f32>(total_h)
            };

            auto born_pos = Tools::calculate_aligned_position(draw_rect, total_w, total_h);

            for (s32 i = 0; i < cur_hp; ++i)
            {
                s32 row = i / hearts_per_line;
                s32 col = i % hearts_per_line;

                SDL_FRect cur_rect {
                    born_pos.x + col * (heart_w + gap),
                    born_pos.y + row * (heart_h + gap),
                    static_cast<f32>(heart_w),
                    static_cast<f32>(heart_h)
                };

                ret = SDL_RenderCopyF(G_GAME.renderer(), heart_texture, NULL, &cur_rect);
            }
        }
    }
    return 0;
}

s32 Object::onDestroy()
{
    // 只有玩家和敌人死亡时有特效
    if (!IsPlayer() && !IsEnemy())
    {
        return 0;
    }

    auto* cur_scene = scene();
    if (! cur_scene)
    {
        LOG_ERROR("cur_scene is null");
        return -1;
    }

    auto* ani = cur_scene->addAnimation<Animation>(cur_scene);
    if (! ani)
    {
        LOG_ERROR("Failed to create animation");
        return -2;
    }

    ani->init("../assets/effect/explosion.png");

    // 设置位置
    auto rect = GetRect();
    auto born_pos = Tools::calculate_aligned_position(rect, ani->width(), ani->height());
    ani->mutable_position()->x = born_pos.x;
    ani->mutable_position()->y = born_pos.y;

    ani->play(1000);

    return 0;
}