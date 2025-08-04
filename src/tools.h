#pragma once

#include <random>
#include <SDL.h>
#include <type_traits>

namespace Tools
{


template<typename T>
T clamp(T value, T min_value, T max_value)
{
    if (value < min_value)
    {
        return min_value;
    }
    else if (value > max_value)
    {
        return max_value;
    }
    
    return value;
}

template<typename T, typename = void>
struct DistributionTraits
{
    using type = std::uniform_int_distribution<T>;
};

template<typename T>
struct DistributionTraits<T, std::enable_if_t<std::is_integral_v<T>>>
{
    using type = std::uniform_int_distribution<T>;
};

template<typename T>
struct DistributionTraits<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
    using type = std::uniform_real_distribution<T>;
};


// min_v <= result <= max_v
// 随机生成函数
template <typename T>
T random(T min_v, T max_v)
{
    if (min_v >= max_v) {
        return min_v;
    }

    using Dist = typename DistributionTraits<T>::type;

    static thread_local std::random_device rd;
    static thread_local std::mt19937_64 rng{rd()};

    if constexpr (std::is_floating_point_v<T>)
    {
        // 为了确保 max_v 有可能被采样到，扩大一点右边界
        Dist dist(min_v, std::nextafter(max_v, std::numeric_limits<T>::max()));
        return dist(rng);
    }
    else
    {
        Dist dist(min_v, max_v); // 整数是闭区间
        return dist(rng);
    }
}



// 获取矩形的中心点
inline SDL_FPoint get_rect_center(const SDL_FRect& rect) {
    SDL_FPoint center;
    center.x = rect.x + rect.w / 2.0f;
    center.y = rect.y + rect.h / 2.0f;
    return center;
}



// 根据一个矩形A和给定的宽高，计算矩形B的左上角位置，使得矩形B与矩形A的中心对齐
inline SDL_FPoint calculate_aligned_position(const SDL_FRect& rect, float w, float h) {
    // 计算矩形A的中心位置
    float center_x = rect.x + rect.w / 2.0f;
    float center_y = rect.y + rect.h / 2.0f;

    // 计算矩形B的左上角位置，使其中心与矩形A对齐
    SDL_FPoint aligned_position;
    aligned_position.x = center_x - w / 2.0f;
    aligned_position.y = center_y - h / 2.0f;

    return aligned_position;
}


// 判断两个矩形是否重叠
inline bool is_rect_overlap(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
    return SDL_HasIntersectionF(&rect1, &rect2);
}


} // namespace Tools