#include "data-comparators.h"
#include "data-extractor.h"
#include "data-processors.h"

namespace
{
    struct LinearInterval
    {
        
    };
    
    [[nodiscard]] std::optional<DataExtractor::DataRangeView> FindLinearInterval(
        const DataRange<float>& inDataRange,
        DataExtractor::DataRangeView InCurrentLinearInterval,
        float inTimeSeconds
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
            if (timeSecondsRight < inTimeSeconds)
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

bool DataComparators::DataComparatorLinear::compareData(
    const DataRange<float>& inDataRange1, 
    const DataRange<float>& inDataRange2,
    const float inTimeOffsetSeconds, 
    std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
) const noexcept
{
    if (inDataProcessors.empty())
    {
        return false;
    }

    for (DataProcessors::DataProcessorBase* const dataProcessor : inDataProcessors)
    {
        dataProcessor->onNewDataBlock();
    }

    DataExtractor::DataRangeView dataRangeView2;
    for (size_t pointIndex = 0; pointIndex < inDataRange1.data.size(); ++pointIndex)
    {
        const float currentTimeSeconds = inDataRange1.timeSeconds[pointIndex] + inTimeOffsetSeconds;
        
        const auto newDataRangeView2Opt = FindLinearInterval(inDataRange2, dataRangeView2, currentTimeSeconds);
        if (newDataRangeView2Opt.has_value())
        {
            dataRangeView2 = newDataRangeView2Opt.value();
        }
        
        const float anotherTimeSecondsLeft = inDataRange2.timeSeconds[dataRangeView2.firstPointIndex];
        const float anotherTimeSecondsRight = inDataRange2.timeSeconds[dataRangeView2.firstPointIndex + 1];
    
        const float timeCoefficient = (currentTimeSeconds - anotherTimeSecondsLeft) / (anotherTimeSecondsRight - anotherTimeSecondsLeft);
            
        const float anotherPointLeft = inDataRange2.data[dataRangeView2.firstPointIndex];
        const float anotherPointRight = inDataRange2.data[dataRangeView2.firstPointIndex + 1];
        const float anotherPoint = anotherPointLeft + (anotherPointRight - anotherPointLeft) * timeCoefficient;

        const float currentPoint = inDataRange1.data[pointIndex];

        for (DataProcessors::DataProcessorBase* const dataProcessor : inDataProcessors)
        {
            dataProcessor->acceptData(currentPoint, anotherPoint, currentTimeSeconds);
        }
    }

    return true;
}

bool DataComparators::compareData(
    const DataRange<float>& inDataRangeLeft, 
    const DataRange<float>& inDataRangeRight, 
    const CompareSettings& inCompareSettings, 
    const DataComparatorBase& inDataComparator,
    std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
) noexcept
{
    if (!DataExtractor::isValid(inDataRangeLeft) || !DataExtractor::isValid(inDataRangeRight))
    {
        return false;
    }

    const std::optional<DataExtractor::TimeRange> timeRange1 = DataExtractor::getTimeRange(inDataRangeLeft);
    const std::optional<DataExtractor::TimeRange> timeRange2 = DataExtractor::getTimeRange(inDataRangeRight);
    if (!timeRange1.has_value() || !timeRange2.has_value())
    {
        return false;
    }

    const float durationDifferenceSeconds = std::abs(timeRange2->endSeconds - timeRange1->endSeconds);
    if (durationDifferenceSeconds > inCompareSettings.maxTimeDifferenceSeconds)
    {
        return false;
    }

    const float timeOffsetSeconds = timeRange1->startSeconds - timeRange2->startSeconds;
    if (!inDataComparator.compareData(inDataRangeLeft, inDataRangeRight, timeOffsetSeconds, inDataProcessors))
    {
        return false;
    }
    if (!inDataComparator.compareData(inDataRangeRight, inDataRangeLeft, -timeOffsetSeconds, inDataProcessors))
    {
        return false;
    }

    return true;
}