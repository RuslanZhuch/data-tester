#include "data-comparators.h"
#include "data-extractor.h"
#include "data-processors.h"

#include <string>

namespace
{
    struct LinearInterval
    {
        
    };
    
    [[nodiscard]] std::optional<DataExtractor::DataRangeView> FindLinearInterval(
        const DataRange<float>& inDataRange,
        DataExtractor::DataRangeView InCurrentLinearInterval,
        float inTimeSeconds,
        float inPreviusTimeSeconds
    )
    {
        std::optional<DataExtractor::DataRangeView> NewLinearInterval;
        if (InCurrentLinearInterval.numOfPoints <= 0)
        {
            const bool isExtracted = DataExtractor::extractNextDataPoints(inDataRange, 2, InCurrentLinearInterval);
            if (!isExtracted)
            {
                return NewLinearInterval;
            }
            NewLinearInterval = InCurrentLinearInterval;
        }
        
        while (true)
        {
            const size_t index = InCurrentLinearInterval.firstPointIndex + 1;
            if (index >= inDataRange.timeSeconds.size())
            {
                return NewLinearInterval;
            }
            
            const float timeSecondsRight = inDataRange.timeSeconds[index];
            if (timeSecondsRight < inTimeSeconds || inPreviusTimeSeconds == timeSecondsRight)
            {
                const bool isExtracted = DataExtractor::extractNextDataPoints(inDataRange, 2, InCurrentLinearInterval);
                if (isExtracted)
                {
                    NewLinearInterval = InCurrentLinearInterval;
                    continue;
                }
                return NewLinearInterval;
            }
            
            return NewLinearInterval;
        }
    }
    
}

std::string DataComparators::DataComparatorLinear::compareData(
    const DataRange<float>& inDataRange1, 
    const DataRange<float>& inDataRange2,
    const float inTimeOffsetSeconds, 
    std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
) const
{
    if (inDataProcessors.empty())
    {
        return "No data processors provided";
    }

    for (DataProcessors::DataProcessorBase* const dataProcessor : inDataProcessors)
    {
        dataProcessor->onNewDataBlock();
    }

    float previousTimeSeconds = -1.f;
    
    DataExtractor::DataRangeView dataRangeView2;
    for (size_t pointIndex = 0; pointIndex < inDataRange1.data.size(); ++pointIndex)
    {
        const float currentTimeSeconds = inDataRange1.timeSeconds[pointIndex] + inTimeOffsetSeconds;
        
        const auto newDataRangeView2Opt = FindLinearInterval(inDataRange2, dataRangeView2, currentTimeSeconds, previousTimeSeconds);
        previousTimeSeconds = currentTimeSeconds;
        if (newDataRangeView2Opt.has_value())
        {
            dataRangeView2 = newDataRangeView2Opt.value();
        }
        
        const float anotherTimeSecondsLeft = inDataRange2.timeSeconds[dataRangeView2.firstPointIndex];
        const float anotherTimeSecondsRight = inDataRange2.timeSeconds[dataRangeView2.firstPointIndex + 1];
    
        //TODO: If anotherTimeSecondsRight == anotherTimeSecondsLeft -> NaN
        float timeCoefficient = 0.f;
        if (anotherTimeSecondsRight != anotherTimeSecondsLeft) 
        {
            timeCoefficient = (currentTimeSeconds - anotherTimeSecondsLeft) / (anotherTimeSecondsRight - anotherTimeSecondsLeft);
        }
            
        const float anotherPointLeft = inDataRange2.data[dataRangeView2.firstPointIndex];
        const float anotherPointRight = inDataRange2.data[dataRangeView2.firstPointIndex + 1];
        const float anotherPoint = anotherPointLeft + (anotherPointRight - anotherPointLeft) * timeCoefficient;

        const float currentPoint = inDataRange1.data[pointIndex];

        for (DataProcessors::DataProcessorBase* const dataProcessor : inDataProcessors)
        {
            dataProcessor->acceptData(currentPoint, anotherPoint, currentTimeSeconds);
        }
    }

    return {};
}

std::string DataComparators::compareData(
    const DataRange<float>& inDataRangeLeft, 
    const DataRange<float>& inDataRangeRight, 
    const CompareSettings& inCompareSettings, 
    const DataComparatorBase& inDataComparator,
    std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
)
{
    if (!DataExtractor::isValid(inDataRangeLeft))
    {
        return "Invalid first data range";
    }
    if (!DataExtractor::isValid(inDataRangeRight))
    {
        return "Invalid second data range";
    }

    const std::optional<DataExtractor::TimeRange> timeRange1 = DataExtractor::getTimeRange(inDataRangeLeft);
    if (!timeRange1.has_value())
    {
        return "Unable to get time range for first data range";
    }
    const std::optional<DataExtractor::TimeRange> timeRange2 = DataExtractor::getTimeRange(inDataRangeRight);
    if (!timeRange2.has_value())
    {
        return "Unable to get time range for second data range";
    }

    const float durationDifferenceSeconds = std::abs(timeRange2->endSeconds - timeRange1->endSeconds);
    if (durationDifferenceSeconds > inCompareSettings.maxTimeDifferenceSeconds)
    {
        return "Duration difference exceeds max time difference";
    }

    const float timeOffsetSeconds = timeRange1->startSeconds - timeRange2->startSeconds;
    const std::string firstToSecondError = inDataComparator.compareData(inDataRangeLeft, inDataRangeRight, timeOffsetSeconds, inDataProcessors);
    if (!firstToSecondError.empty())
    {
        return "First-to-second comparison failed: " + firstToSecondError;
    }
    const std::string secondToFirstError = inDataComparator.compareData(inDataRangeRight, inDataRangeLeft, -timeOffsetSeconds, inDataProcessors);
    if (!secondToFirstError.empty())
    {
        return "Second-to-first comparison failed: " + secondToFirstError;
    }

    return {};
}
