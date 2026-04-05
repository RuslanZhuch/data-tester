#pragma once
#include "data-holder.h"
#include "data-processors.h"
#include <span>
#include <functional>

class DataProcessors::DataProcessorBase;

namespace DataComparators
{
    struct CompareSettings
    {
        float maxTimeDifferenceSeconds = 0.01f;
    };

    class DataComparatorBase
    {
    public:
        virtual ~DataComparatorBase() = default;
    public:
        virtual bool compareData(const DataRange<float>& inDataRange1, const DataRange<float>& inDataRange2, const float inTimeOffsetSeconds, std::span<DataProcessors::DataProcessorBase* const> inDataProcessors) const noexcept = 0;
    };

    class DataComparatorLinear : public DataComparatorBase
    {
    public:
        bool compareData(const DataRange<float>& inDataRange1, const DataRange<float>& inDataRange2, const float inTimeOffsetSeconds, std::span<DataProcessors::DataProcessorBase* const> inDataProcessors) const noexcept override;
    };

    [[nodiscard]] bool compareData(
        const DataRange<float>& inDataRangeLeft, 
        const DataRange<float>& inDataRangeRight, 
        const CompareSettings& inCompareSettings,
        const DataComparatorBase& inDataComparator,
        std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
    ) noexcept;
}
