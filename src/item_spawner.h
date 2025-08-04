#pragma once

#include "comm_def.h"
#include <random>

class ItemSpawner
{
public:
    ItemSpawner();
    ~ItemSpawner();

    s32 init();
    s32 update(s64 now_ms);

private:
    s32 spawn_item(s64 now_ms);

private:
    DEF_Property_default(s64, last_spawn_time, 0); // 上次生成的时间
    DEF_Property_default(s32, spawn_max_interval, 6000); // 生成最大间隔
    DEF_Property_default(s32, spawn_min_interval, 2000); // 生成最小间隔
    DEF_Property_default(s32, spawn_interval, 500); // 每次生成时， 计算下次的生成间隔
};