#include "enemy_spawner.h"
#include "game.h"
#include "tools.h"
#include "enemy.h"

EnemySpawner::EnemySpawner()
{
}

EnemySpawner::~EnemySpawner()
{
}


s32 EnemySpawner::init()
{
    return 0;
}

s32 EnemySpawner::update(s64 now_ms)
{
    if (now_ms >= last_spawn_time() + spawn_interval())
    {
        spawn_enemy(now_ms);

        // 计算下一次间隔
        auto next_interval = Tools::random(spawn_min_interval(), spawn_max_interval());
        set_spawn_interval(next_interval);

        // 更新最后生成时间
        set_last_spawn_time(now_ms);
    }
    return 0;
}

s32 EnemySpawner::spawn_enemy(s64 now_ms)
{
    LOG_INFO("spawn_enemy...");
    if (G_GAME.current_scene() == nullptr)
    {
        LOG_ERROR("current scene is nullptr");
        return -1;
    }

    auto* curr_scene = G_GAME.current_scene();


    auto* enemy = curr_scene->addObject<Enemy>(curr_scene);
    
    if (! enemy)
    {
        LOG_ERROR("addObject Enemy failed");
        return -2;
    }

    enemy->init("../assets/image/insect-1.png");

    // 设置位置
    s32 random_x = Tools::random(0, G_GAME.window_width() - enemy->width());
    s32 random_y = -enemy->height();

    enemy->mutable_position()->x = random_x;
    enemy->mutable_position()->y = random_y;

    LOG_INFO("spawn_enemy x=%d, y=%d", random_x, random_y);

    return 0;
}
