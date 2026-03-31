#pragma once

#include "data-extractor.h"
#include <unordered_map>
#include <string>
#include <functional>

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
    std::unordered_map<std::string, DataRangeFloat> dataFloats;
};
