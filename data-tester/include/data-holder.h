#pragma once

#include "data-extractor.h"
#include <unordered_map>
#include <string>
#include <functional>

namespace DataComparators
{
    struct CompareSettings;
}

struct DataToInsert
{
    std::unordered_map<std::string, float> dataFloats;
    std::unordered_map<std::string, int> dataInts;

    float timeSeconds{};
};

class DataHolder
{
public:
    using DataRangeFloat = DataRange<float>;
    using DataRangeCallback = std::function<void(std::string_view, const DataRangeFloat&)>;
public:
    DataHolder() = default;

    DataHolder(const DataHolder&) = delete;
    DataHolder& operator=(const DataHolder&) = delete;

    DataHolder(DataHolder&&) noexcept = default;
    DataHolder& operator=(DataHolder&&) noexcept = default;

    ~DataHolder() = default;
public:
    void insertData(const DataToInsert& inDataToInsert);

    void forEachDataRange(const DataRangeCallback& inCallback) const;
private:
    friend class DataIterator;
    std::unordered_map<std::string, DataRangeFloat> dataFloats;
};

class DataIterator
{
public:
    using DataRangeFloat = DataRange<float>;
    using DataRangeCallback = std::function<void(const std::string&, const DataRangeFloat&, const DataComparators::CompareSettings&, const DataRangeFloat&)>;
public:
    void forEachDataRange(const DataHolder& inDataHolder1, const DataHolder& inDataHolder2, const DataComparators::CompareSettings& inCompareSettings, const DataRangeCallback& inCallback);
};

