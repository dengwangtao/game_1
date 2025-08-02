#include "guid_gen.h"
#include "game.h"
#include "comm_def.h"

// | 类型 5bit | 时间戳(s) 32bit | seq 27bit |

constexpr s32 GUID_TYPE_BITS = 5;
constexpr s32 GUID_TIMESTAMP_BITS = 32;
constexpr s32 GUID_SEQ_BITS = 27;

constexpr u64 GUID_TYPE_MASK = (1ULL << GUID_TYPE_BITS) - 1;
constexpr u64 GUID_TIMESTAMP_MASK = (1ULL << GUID_TIMESTAMP_BITS) - 1;
constexpr u64 GUID_SEQ_MASK = (1ULL << GUID_SEQ_BITS) - 1;

constexpr s32 GUID_TYPE_SHIFT = GUID_SEQ_BITS + GUID_TIMESTAMP_BITS;
constexpr s32 GUID_TIMESTAMP_SHIFT = GUID_SEQ_BITS;
constexpr s32 GUID_SEQ_SHIFT = 0;

constexpr s64 time20250801 = 1753977600; // 2025-08-01 00:00:00 UTC+8


u64 GUIDGen::GenerateGUID(GUID_TYPE type)
{
    static s64 cur_s = 0; // 当前正在用的秒
    static u32 cur_seq = 0; // 当前秒内的序列号

    s64 now_s = G_GAME.now_ms() / 1000; // 当前秒

    if (now_s > cur_s)
    {
        cur_seq = 0;
        cur_s = now_s;
        LOG_INFO("New second: %lld", cur_s);
    }
    else if (cur_seq >= GUID_SEQ_MASK)
    {
        cur_s ++;
        cur_seq = 0;
        LOG_INFO("New second: %lld", cur_s);
    }
    else
    {
        ++ cur_seq;
        LOG_INFO("New seq: %d now_s: %lld cur_s: %lld cur_seq: %d", cur_seq, now_s, cur_s, cur_seq);
    }

    u64 rel_s = cur_s - time20250801; // 相对时间戳

    u64 guid = 0;
    guid |= (static_cast<u64>(type) & GUID_TYPE_MASK) << GUID_TYPE_SHIFT;
    guid |= (static_cast<u64>(rel_s) & GUID_TIMESTAMP_MASK) << GUID_TIMESTAMP_SHIFT;
    guid |= (static_cast<u64>(cur_seq) & GUID_SEQ_MASK) << GUID_SEQ_SHIFT;

    return guid;
}

std::string GUIDGen::ParseGUID(u64 guid)
{
    GUID_TYPE type = static_cast<GUID_TYPE>((guid >> GUID_TYPE_SHIFT) & GUID_TYPE_MASK);
    s64 rel_s = (guid >> GUID_TIMESTAMP_SHIFT) & GUID_TIMESTAMP_MASK;
    u32 seq = (guid >> GUID_SEQ_SHIFT) & GUID_SEQ_MASK;
    s64 abs_s = rel_s + time20250801; // 绝对时间戳

    std::string str = std::string(GUID_TYPE_STR[type]) + "-" + std::to_string(abs_s) + "-" + std::to_string(seq);
    return str;
}