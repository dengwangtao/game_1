#pragma once

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


} // namespace Tools