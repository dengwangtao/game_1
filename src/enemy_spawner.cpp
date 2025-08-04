#include "enemy_spawner.h"
#include "game.h"
#include "tools.h"

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
    return 0;
}
