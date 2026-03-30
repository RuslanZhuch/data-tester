#pragma once

#include <vector>

template <typename T>
struct DataRange
{
    struct DataWithTime
    {
        T data;
        float timeSeconds;
    };

    std::vector<T> data;
    std::vector<float> timeSeconds;
};
