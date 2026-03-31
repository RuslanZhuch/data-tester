#pragma once

#include <vector>

template <typename T>
struct DataRange
{
    struct DataWithTime
    {
        T data;
        float timeSeconds;
        constexpr auto operator<=>(const DataWithTime& inOther) const = default;
    };

    std::vector<T> data;
    std::vector<float> timeSeconds;
    
    constexpr auto operator<=>(const DataRange& inOther) const = default;
};
