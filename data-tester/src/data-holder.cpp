#include "data-holder.h"

#include <algorithm>
#include <ranges>
#include <set>

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

void DataIterator::forEachDataRange(const DataHolder& inDataHolder1, const DataHolder& inDataHolder2, const DataComparators::CompareSettings& inCompareSettings, const DataRangeCallback& inCallback)
{
    // TODO: DataHolder's data structure is need to be replaced
    if (!inCallback)
    {
        return;
    }
    
    std::set<std::string> iteratedEntries;
    for (const std::pair<const std::string, DataRange<float>>& entry1 : inDataHolder1.dataFloats)
    {
        if (iteratedEntries.contains(entry1.first))
        {
            continue;
        }
        iteratedEntries.insert(entry1.first);
        
        const auto& entry2 = inDataHolder2.dataFloats.find(entry1.first);
        if (entry2 == inDataHolder2.dataFloats.end())
        {
            inCallback(entry1.first, entry1.second, inCompareSettings, DataRangeFloat{});
            continue;
        }
        
        inCallback(entry1.first, entry1.second, inCompareSettings, entry2->second);
    }
    for (const std::pair<const std::string, DataRange<float>>& entry1 : inDataHolder2.dataFloats)
    {
        if (iteratedEntries.contains(entry1.first))
        {
            continue;
        }
        iteratedEntries.insert(entry1.first);
        
        const auto& entry2 = inDataHolder1.dataFloats.find(entry1.first);
        if (entry2 == inDataHolder1.dataFloats.end())
        {
            inCallback(entry1.first, entry1.second, inCompareSettings, DataRangeFloat{});
            continue;
        }
        
        inCallback(entry1.first, entry1.second, inCompareSettings, entry2->second);
    }
}
