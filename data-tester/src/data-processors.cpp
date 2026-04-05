#include "data-processors.h"
#include <format>

void DataProcessors::DataProcessorMeanSquaredError::acceptData(float inPoint1, float inPoint2, [[maybe_unused]] float inTimeSeconds) noexcept
{
    sumOfSquares += (inPoint1 - inPoint2) * (inPoint1 - inPoint2);
    ++numOfErrors;
}

void DataProcessors::DataProcessorMeanSquaredError::extractIssues(issues_t& inOutProblemsData) const noexcept
{
    const float meanSquaredError = getMeanSquaredError();
    
    if (meanSquaredError > desc.maxMeanSquaredError)
    {
        std::string problem = std::format(R"(Mean squared error {:.4f} is greater than {}. )", meanSquaredError, desc.maxMeanSquaredError);
        inOutProblemsData.append(problem);
    }
}

float DataProcessors::DataProcessorMeanSquaredError::getMeanSquaredError() const noexcept
{
    if (numOfErrors == 0)
    {
        return 0.f;
    }
    return sumOfSquares / static_cast<float>(numOfErrors);
}

void DataProcessors::DataProcessorMaxAbsoluteError::acceptData(float inPoint1, float inPoint2, [[maybe_unused]] float inTimeSeconds) noexcept
{
    const float NewAbsoluteError = std::abs(inPoint1 - inPoint2);
    if (NewAbsoluteError > currentMaxAbsoluteError)
    {
        maxErrorDataIndex = currentDataIndex;
        maxErrorTime = inTimeSeconds;
        currentMaxAbsoluteError = NewAbsoluteError;
    }
    ++currentDataIndex;
}

void DataProcessors::DataProcessorMaxAbsoluteError::extractIssues(issues_t& inOutProblemsData) const noexcept
{
    if (currentMaxAbsoluteError > desc.maxAbsoluteError)
    {
        std::string problem = std::format(R"(Absolute error {:.4f} is greater than {}, time: {:.4f}, dataIndex: {}. )", 
            currentMaxAbsoluteError, 
            desc.maxAbsoluteError,
            maxErrorTime,
            maxErrorDataIndex
        );
        inOutProblemsData.append(problem);
    }
}

void DataProcessors::DataProcessorMaxAbsoluteError::onNewDataBlock() noexcept
{
    currentDataIndex = {};
}
