#pragma once

#include "data-extractor.h"
#include <unordered_map>
#include <string>

struct DataToInsert
{
    std::unordered_map<std::string, float> dataFloats;
    std::unordered_map<std::string, int> dataInts;

    float timeSeconds{};
};

class DataHolder
{
public:
    using DataRangeFloat = DataRange<float>::DataWithTime;
    using DataRangeInt = DataRange<int>::DataWithTime;
public:
    DataHolder() = default;

    DataHolder(const DataHolder&) = delete;
    DataHolder& operator=(const DataHolder&) = delete;

    DataHolder(DataHolder&&) noexcept = default;
    DataHolder& operator=(DataHolder&&) noexcept = default;

    ~DataHolder() = default;
public:
    void insertData(const DataToInsert& dataToInsert);
    void insertData(DataToInsert&& dataToInsert);

    [[nodiscard]] const DataRangeFloat& getDataRangeFloat(std::string_view name) const;
    [[nodiscard]] const DataRangeInt& getDataRangeInt(std::string_view name) const;
private:
    std::unordered_map<std::string, DataRangeFloat> dataFloats;
    std::unordered_map<std::string, DataRangeInt> dataInts;
};
