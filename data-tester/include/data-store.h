#pragma once

#include <filesystem>


class DataHolder;

class DataStoreBinary
{
public:
    bool saveData(const std::filesystem::path& inFilePath, const DataHolder& inDataHolder) const;
    bool loadData(const std::filesystem::path& inFilePath, DataHolder& outDataHolder) const;
};
