#pragma once

#include <random>
#include <SDL.h>

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


// min_v <= result <= max_v
template <typename T, typename DS=std::uniform_int_distribution<T>>
T random(T min_v, T max_v)
{
    if (min_v >= max_v)
    {
        return min_v;
    }
    static std::random_device rd;
    static std::mt19937_64 e{rd()};
    DS dist{min_v, max_v};

    return dist(e);
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



} // namespace Tools