#include <gtest/gtest.h>
#include <filesystem>

#include "data-holder.h"
#include "data-store.h"

namespace DataStoreTest
{
    class DataStoreFixture : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            tempDir = std::filesystem::path(__FILE__).parent_path().parent_path() / "temp";
            resourceDir = std::filesystem::path(__FILE__).parent_path().parent_path() / "resources";
            std::filesystem::remove_all(tempDir);
            std::filesystem::create_directory(tempDir);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(tempDir);
        }

        std::filesystem::path filePath(std::string_view inFileName) const
        {
            return tempDir / inFileName;
        }
        
        std::filesystem::path resourceFilePath(std::string_view inFileName) const
        {
            return resourceDir / inFileName;
        }

        std::filesystem::path tempDir;
        std::filesystem::path resourceDir;
        DataStoreBinary dataStoreBinary;
    };

    TEST_F(DataStoreFixture, Empty)
    {
        DataHolder dataHolder;
        const bool bIsSaved = dataStoreBinary.saveData(filePath("test-data-store"), dataHolder);
        ASSERT_TRUE(bIsSaved);

        DataHolder loadedDataHolder;

        const bool bIsLoaded = dataStoreBinary.loadData(filePath("test-data-store"), loadedDataHolder);
        ASSERT_TRUE(bIsLoaded);

        EXPECT_EQ(dataHolder, loadedDataHolder);
    }

    TEST_F(DataStoreFixture, LoadEmpty)
    {
        DataHolder loadedDataHolder;
        const bool bIsLoaded = dataStoreBinary.loadData(resourceFilePath("empty-data"), loadedDataHolder);
        ASSERT_FALSE(bIsLoaded);
    }
    
    TEST_F(DataStoreFixture, WrongVersion)
    {
        DataHolder loadedDataHolder;
        const bool bIsLoaded = dataStoreBinary.loadData(resourceFilePath("wrong-version-data"), loadedDataHolder);
        ASSERT_FALSE(bIsLoaded);
    }
    
    TEST_F(DataStoreFixture, CorruptedData)
    {
        DataHolder loadedDataHolder;
        const bool bIsLoaded = dataStoreBinary.loadData(resourceFilePath("wrong-version-data"), loadedDataHolder);
        ASSERT_FALSE(bIsLoaded);
    }

    TEST_F(DataStoreFixture, SaveAndLoad)
    {
        DataHolder dataHolder;

        DataToInsert data;
        data.dataFloats["data_1"] = 1.f;
        data.dataFloats["data_2"] = 2.f;
        data.dataFloats["data_3"] = 3.f;
        data.dataFloats["data_4"] = 4.f;
        data.timeSeconds = 100.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_1"] = 5.f;
        data.dataFloats["data_2"] = 6.f;
        data.dataFloats["data_3"] = 7.f;
        data.dataFloats["data_4"] = 8.f;
        data.timeSeconds = 101.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_1"] = 9.f;
        data.dataFloats["data_2"] = 10.f;
        data.dataFloats["data_3"] = 11.f;
        data.dataFloats["data_4"] = 12.f;
        data.timeSeconds = 102.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_1"] = 13.f;
        data.dataFloats["data_2"] = 14.f;
        data.dataFloats["data_3"] = 15.f;
        data.dataFloats["data_4"] = 16.f;
        data.timeSeconds = 113.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_5"] = 17.f;
        data.dataFloats["data_6"] = 18.f;
        data.timeSeconds = 114.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_2"] = 19.f;
        data.dataFloats["data_3"] = 20.f;
        data.dataFloats["data_4"] = 21.f;
        data.timeSeconds = 115.f;
        dataHolder.insertData(data);
        
        data = {};
        data.dataFloats["data_1"] = 22.f;
        data.dataFloats["data_2"] = 23.f;
        data.dataFloats["data_3"] = 24.f;
        data.dataFloats["data_4"] = 25.f;
        data.timeSeconds = 116.f;
        dataHolder.insertData(data);

        const bool bIsSaved = dataStoreBinary.saveData(filePath("test-data-store"), dataHolder);
        ASSERT_TRUE(bIsSaved);
        
        DataHolder loadedDataHolder;
        data = {};
        data.dataFloats["data_1"] = 26.f;
        data.dataFloats["data_2"] = 27.f;
        data.dataFloats["data_3"] = 28.f;
        data.dataFloats["data_4"] = 29.f;
        data.timeSeconds = 117.f;
        loadedDataHolder.insertData(data);
        
        const bool bIsLoaded = dataStoreBinary.loadData(filePath("test-data-store"), loadedDataHolder);
        ASSERT_TRUE(bIsLoaded);

        EXPECT_EQ(dataHolder, loadedDataHolder);
    }
}
