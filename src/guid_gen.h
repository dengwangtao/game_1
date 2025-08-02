#pragma once

#include "comm_def.h"

#include <string>

enum GUID_TYPE
{
    GUID_PLAYER = 0,
    GUID_ENEMY, // 1
    GUID_BULLET, // 2
};

constexpr const char* GUID_TYPE_STR[] = { "PLAYER", "ENEMY", "BULLET" };

class GUIDGen
{
public:
    static u64 GenerateGUID(GUID_TYPE type);
    static std::string ParseGUID(u64 guid);
};