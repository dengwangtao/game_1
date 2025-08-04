#pragma once

#include <random>

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

} // namespace Tools