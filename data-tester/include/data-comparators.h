#pragma once
#include "data-holder.h"
#include "data-processors.h"
#include <span>
#include <functional>
#include <string>

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
        [[nodiscard]] virtual std::string compareData(const DataRange<float>& inDataRange1, const DataRange<float>& inDataRange2, const float inTimeOffsetSeconds, std::span<DataProcessors::DataProcessorBase* const> inDataProcessors) const = 0;
    };

    class DataComparatorLinear : public DataComparatorBase
    {
    public:
        [[nodiscard]] std::string compareData(const DataRange<float>& inDataRange1, const DataRange<float>& inDataRange2, const float inTimeOffsetSeconds, std::span<DataProcessors::DataProcessorBase* const> inDataProcessors) const override;
    };

    [[nodiscard]] std::string compareData(
        const DataRange<float>& inDataRangeLeft, 
        const DataRange<float>& inDataRangeRight, 
        const CompareSettings& inCompareSettings,
        const DataComparatorBase& inDataComparator,
        std::span<DataProcessors::DataProcessorBase* const> inDataProcessors
    );
}
