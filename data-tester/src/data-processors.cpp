#include "data-processors.h"

void DataProcessors::DataProcessorMeanSquaredError::acceptData(const float inPoint1, const float inPoint2, [[maybe_unused]] const float inTimeSeconds) noexcept
{
    sumOfSquares += (inPoint1 - inPoint2) * (inPoint1 - inPoint2);
    ++numOfErrors;
}

float DataProcessors::DataProcessorMeanSquaredError::getMeanSquaredError() const noexcept
{
    if (numOfErrors == 0)
    {
        return 0.f;
    }
    return sumOfSquares / static_cast<float>(numOfErrors);
}

void DataProcessors::DataProcessorMaxAbsoluteError::acceptData(const float inPoint1, const float inPoint2, [[maybe_unused]] const float inTimeSeconds) noexcept
{
    const float NewAbsoluteError = std::abs(inPoint1 - inPoint2);
    maxAbsoluteError = std::max(maxAbsoluteError, NewAbsoluteError);
}

std::optional<float> DataProcessors::DataProcessorMaxAbsoluteError::getMaxAbsoluteError() const noexcept
{
    if (maxAbsoluteError < 0)
    {
        return std::nullopt;
    }
    
    return maxAbsoluteError;
}
