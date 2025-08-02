#pragma once

#include "comm_def.h"
#include "object_type.h"

#include <string>


class GUIDGen
{
public:
    static u64 GenerateGUID(ObjectType type);
    static std::string ParseGUID(u64 guid);
};