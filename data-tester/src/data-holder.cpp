#include "data-holder.h"

#include <algorithm>
#include <ranges>

namespace
{
    void insertDataImpl(DataRange<float>& inOutDataRange, float inData, float inTimeSeconds)
    {
        if (inOutDataRange.timeSeconds.empty() || inOutDataRange.timeSeconds.back() <= inTimeSeconds)
        {
            inOutDataRange.data.push_back(inData);
            inOutDataRange.timeSeconds.push_back(inTimeSeconds);
        }
        else
        {
            const auto& lowerBound = std::ranges::lower_bound(inOutDataRange.timeSeconds, inTimeSeconds);
            const ptrdiff_t insertIndex = std::distance(inOutDataRange.timeSeconds.begin(), lowerBound);
            inOutDataRange.data.insert(inOutDataRange.data.begin() + insertIndex, inData);
            inOutDataRange.timeSeconds.insert(inOutDataRange.timeSeconds.begin() + insertIndex, inTimeSeconds);
        }
    }
}

void DataHolder::insertData(const DataToInsert& inDataToInsert)
{
    const float timeSeconds = inDataToInsert.timeSeconds;
    for (const std::pair<const std::string, float>& data : inDataToInsert.dataFloats)
    {
        DataRange<float>& dataRange = this->dataFloats[data.first];
        insertDataImpl(dataRange, data.second, timeSeconds);
    }
    
    for (const std::pair<const std::string, int32_t>& data : inDataToInsert.dataInts)
    {
        DataRange<float>& dataRange = this->dataFloats[data.first];
        insertDataImpl(dataRange, static_cast<float>(data.second), timeSeconds);
    }
}

void DataHolder::forEachDataRange(const DataRangeCallback& inCallback) const
{
    if (!inCallback)
    {
        return;
    }
    
    for (const std::pair<const std::string, DataRange<float>>& data : this->dataFloats)
    {
        inCallback(data.first, data.second);
    }
}
