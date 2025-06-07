#pragma once

namespace std
{
    template <typename T>
    T wrap(T value, T min, T max) {
        T range = max - min;
        if (range == 0) return min; // avoid division by zero
        while (value < min) value += range;
        while (value >= max) value -= range;
        return value;
    }

    // float approximately(float first, float second)
    // {
    //     first 
    // }
}
