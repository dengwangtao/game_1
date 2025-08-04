#include "item_spawner.h"
#include "game.h"
#include "tools.h"
#include "item.h"

ItemSpawner::ItemSpawner()
{
}

ItemSpawner::~ItemSpawner()
{
}


s32 ItemSpawner::init()
{
    return 0;
}

s32 ItemSpawner::update(s64 now_ms)
{
    if (now_ms >= last_spawn_time() + spawn_interval())
    {
        spawn_item(now_ms);

        // 计算下一次间隔
        auto next_interval = Tools::random(spawn_min_interval(), spawn_max_interval());
        set_spawn_interval(next_interval);

        // 更新最后生成时间
        set_last_spawn_time(now_ms);
    }
    return 0;
}

s32 ItemSpawner::spawn_item(s64 now_ms)
{
    LOG_INFO("spawn_item...");
    if (G_GAME.current_scene() == nullptr)
    {
        LOG_ERROR("current scene is nullptr");
        return -1;
    }

    auto* curr_scene = G_GAME.current_scene();


    auto* item = curr_scene->addObject<Item>(curr_scene);
    
    if (! item)
    {
        LOG_ERROR("addObject Item failed");
        return -2;
    }

    item->init("../assets/image/bonus_life.png");

    // 设置位置
    s32 random_x = Tools::random(0, G_GAME.window_width() - item->width());
    s32 random_y = -item->height();

    item->mutable_position()->x = random_x;
    item->mutable_position()->y = random_y;

    LOG_INFO("spawn_item x=%d, y=%d", random_x, random_y);

    return 0;
}
