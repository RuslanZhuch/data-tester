#pragma once

#include "data-holder.h"
#include <optional>
#include <cstdint>

namespace DataExtractor
{
    template <typename T>
    [[nodiscard]] bool isEmpty([[maybe_unused]] const DataRange<T>& inDataRange) noexcept
    {
        return inDataRange.data.empty() || inDataRange.timeSeconds.empty();
    }
    
    template <typename T>
    [[nodiscard]] bool isValid([[maybe_unused]] const DataRange<T>& inDataRange) noexcept
    {
        if (inDataRange.data.size() != inDataRange.timeSeconds.size())
        {
            return false;
        }
        
        return !inDataRange.data.empty();
    }
    
    using DataRangeFloat = DataRange<float>;
    using DataRangeInt = DataRange<int>;

    struct DataRangeView
    {
        int32_t firstPointIndex{};
        int32_t numOfPoints{};

        [[nodiscard]] auto operator<=>(const DataRangeView&) const noexcept = default;
    };

    template <typename T>
    [[nodiscard]] bool extractNextDataPoints([[maybe_unused]] const DataRange<T>& inDataRange, [[maybe_unused]] int32_t inNumOfPointsToExtract, [[maybe_unused]] DataRangeView& inOutPreviousDataPoints) noexcept
    {
        if (inNumOfPointsToExtract <= 0)
        {
            return false;
        }
        
        const bool bWasUsed = inOutPreviousDataPoints.numOfPoints > 0;
        const int32_t newFirstPointIndex = inOutPreviousDataPoints.firstPointIndex + bWasUsed;
        const int32_t newLastPointIndex = newFirstPointIndex + inNumOfPointsToExtract - 1;
        
        if (newLastPointIndex >= inDataRange.timeSeconds.size())
        {
            return false;
        }
        
        inOutPreviousDataPoints.firstPointIndex = newFirstPointIndex;
        inOutPreviousDataPoints.numOfPoints = inNumOfPointsToExtract;
        return true;
    }

//    [[nodiscard]] std::optional<DataRangeFloat::DataWithTime> getApproximatedData(const DataRangeFloat& inDataRange, float inTimeSeconds) noexcept;
//    [[nodiscard]] std::optional<DataRangeInt::DataWithTime> getApproximatedData(const DataRangeInt& inDataRange, float inTimeSeconds) noexcept;

    struct TimeRange
    {
        float startSeconds;
        float endSeconds;
    };

    [[nodiscard]] std::optional<TimeRange> getTimeRange(const DataRangeFloat& inDataRange) noexcept;
    [[nodiscard]] std::optional<TimeRange> getTimeRange(const DataRangeInt& inDataRange) noexcept;
};
