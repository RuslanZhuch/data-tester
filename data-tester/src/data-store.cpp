#include "data-store.h"
#include "data-holder.h"

#include <fstream>
#include <cstdint>

bool DataStoreBinary::saveData(const std::filesystem::path& inFilePath, const DataHolder& inDataHolder) const
{
    std::ofstream file(inFilePath.string() + ".sent", std::ios::binary);
    if (!file)
    {
        return false;
    }

    const uint16_t version = 1;
    file.write(reinterpret_cast<const char*>(&version), sizeof(version));

    for (const auto& [name, range] : inDataHolder.dataFloats)
    {
        const uint8_t nameSize = static_cast<uint8_t>(name.size());
        file.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        file.write(name.data(), nameSize);

        const uint32_t numElements = static_cast<uint32_t>(range.data.size());
        file.write(reinterpret_cast<const char*>(&numElements), sizeof(numElements));

        const std::streamsize numOfBytes = static_cast<std::streamsize>(range.data.size()) * 4;
        file.write(reinterpret_cast<const char*>(range.data.data()), numOfBytes);
        file.write(reinterpret_cast<const char*>(range.timeSeconds.data()), numOfBytes);
    }

    return file.good();
}

bool DataStoreBinary::loadData(const std::filesystem::path& inFilePath, DataHolder& outDataHolder) const
{
    outDataHolder.dataFloats.clear();
    
    std::ifstream file(inFilePath.string() + ".sent", std::ios::binary);
    if (!file)
    {
        return false;
    }

    uint16_t version = 0;
    file.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (!file || version != 1)
    {
        return false;
    }

    while (file.peek() != EOF)
    {
        uint8_t nameSize = 0;
        file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        if (!file)
        {
            break;
        }

        std::string name(nameSize, '\0');
        file.read(name.data(), nameSize);
        if (!file)
            return false;

        uint32_t numElements = 0;
        file.read(reinterpret_cast<char*>(&numElements), sizeof(numElements));
        if (!file)
        {
            return false;
        }

        DataHolder::DataRangeFloat range;
        range.data.resize(numElements);
        range.timeSeconds.resize(numElements);

        const std::streamsize numOfBytes = static_cast<std::streamsize>(range.data.size()) * 4;
        file.read(reinterpret_cast<char*>(range.data.data()), numOfBytes);
        if (!file)
        {
            return false;
        }

        file.read(reinterpret_cast<char*>(range.timeSeconds.data()), numOfBytes);
        if (!file)
        {
            return false;
        }

        outDataHolder.dataFloats[std::move(name)] = std::move(range);
    }

    return true;
}
