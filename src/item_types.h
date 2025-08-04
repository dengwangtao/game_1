

#include "comm_def.h"
#include <string_view>

enum ItemType
{
    ITEM_NONE,
    ITEM_HEALTH,
    ITEM_SHIELD,
    ITEM_TIME,
    ITEM_MAX
};

constexpr s32 ITEM_TYPE_MIN = ITEM_NONE + 1;
constexpr s32 ITEM_TYPE_MAX = ITEM_MAX - 1;

template<ItemType type>
struct ItemTraits
{
    static constexpr std::string_view name = "unknown";
    static constexpr std::string_view fname = "unknown";
};


template<>
struct ItemTraits<ITEM_HEALTH>
{
    static constexpr std::string_view name = "health potion";
    static constexpr std::string_view fname = "../assets/image/bonus_life.png";
};

template<>
struct ItemTraits<ITEM_SHIELD>
{
    static constexpr std::string_view name = "armor potion";
    static constexpr std::string_view fname = "../assets/image/bonus_shield.png";
};

template<>
struct ItemTraits<ITEM_TIME>
{
    static constexpr std::string_view name = "time potion";
    static constexpr std::string_view fname = "../assets/image/bonus_time.png";
};

namespace ItemTypes
{

inline constexpr std::string_view GetItemName(ItemType type)
{
    switch (type)
    {
        case ITEM_HEALTH: return ItemTraits<ITEM_HEALTH>::name;
        case ITEM_SHIELD: return ItemTraits<ITEM_SHIELD>::name;
        case ITEM_TIME:   return ItemTraits<ITEM_TIME>::name;
        default:          return ItemTraits<ITEM_NONE>::name;
    }
}
inline constexpr std::string_view GetItemFile(ItemType type)
{
    switch (type)
    {
        case ITEM_HEALTH: return ItemTraits<ITEM_HEALTH>::fname;
        case ITEM_SHIELD: return ItemTraits<ITEM_SHIELD>::fname;
        case ITEM_TIME:   return ItemTraits<ITEM_TIME>::fname;
        default:          return ItemTraits<ITEM_NONE>::fname;
    }
}

} // namespace ItemTypes