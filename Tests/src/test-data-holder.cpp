#include <gtest/gtest.h>

#include "data-holder.h"

namespace Providers
{
    struct DataCollectorData
    {
        std::unordered_map<std::string, DataRange<float>> collectedData;

        constexpr auto operator<=>(const DataCollectorData& inOther) const = default;
    };
    class DataCollector
    {
    public:
        DataCollector(): 
            data(std::make_shared<DataCollectorData>())
        {
        }
        
        void operator()(std::string_view inDataName, const DataRange<float>& inDataRange) const
        {
            data->collectedData.emplace(inDataName, inDataRange);
        }
    public:
        std::shared_ptr<DataCollectorData> data;
    };    
}

namespace DataHolderTest
{
    TEST(DataHolder, Empty)
    {
        DataHolder dataHolder;

        Providers::DataCollector collector;
        auto data = collector.data;
        dataHolder.forEachDataRange(collector);

        EXPECT_TRUE(data->collectedData.empty());
        dataHolder.forEachDataRange({});
    }
    
    TEST(DataHolder, DataEntries)
    {
        DataHolder dataHolder;

        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 1.0f;
            dataToInsert.dataFloats["data2"] = 2.0f;
            dataToInsert.timeSeconds = 100.0f;
            dataHolder.insertData(dataToInsert);
            
            Providers::DataCollector collector;
            auto data = collector.data;
            dataHolder.forEachDataRange(collector);
            EXPECT_EQ(data->collectedData.size(), 2);
            EXPECT_TRUE(data->collectedData.contains("data1"));
            EXPECT_TRUE(data->collectedData.contains("data2"));
            EXPECT_FALSE(data->collectedData.contains("data3"));
            dataHolder.forEachDataRange({});
        }

        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data2"] = 3.0f;
            dataToInsert.timeSeconds = 200.0f;
            dataHolder.insertData(dataToInsert);
            
            Providers::DataCollector collector;
            auto data = collector.data;
            dataHolder.forEachDataRange(collector);
            EXPECT_EQ(data->collectedData.size(), 2);
            EXPECT_TRUE(data->collectedData.contains("data1"));
            EXPECT_TRUE(data->collectedData.contains("data2"));
            EXPECT_FALSE(data->collectedData.contains("data3"));
        }

        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 4.0f;
            dataToInsert.dataFloats["data3"] = 5.0f;
            dataToInsert.timeSeconds = 300.0f;
            dataHolder.insertData(dataToInsert);
            
            Providers::DataCollector collector;
            auto data = collector.data;
            dataHolder.forEachDataRange(collector);
            EXPECT_EQ(data->collectedData.size(), 3);
            EXPECT_TRUE(data->collectedData.contains("data1"));
            EXPECT_TRUE(data->collectedData.contains("data2"));
            EXPECT_TRUE(data->collectedData.contains("data3"));
        }

        {
            DataToInsert dataToInsert;
            dataToInsert.timeSeconds = 400.0f;
            dataHolder.insertData(dataToInsert);
            
            Providers::DataCollector collector;
            auto data = collector.data;
            dataHolder.forEachDataRange(collector);
            EXPECT_EQ(data->collectedData.size(), 3);
            EXPECT_TRUE(data->collectedData.contains("data1"));
            EXPECT_TRUE(data->collectedData.contains("data2"));
            EXPECT_TRUE(data->collectedData.contains("data3"));
        }
    }
    
    TEST(DataHolder, Normal)
    {
        DataHolder dataHolder;
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 1.0f;
            dataToInsert.dataInts["data2"] = 2;
            dataToInsert.timeSeconds = 100.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 3.0f;
            dataToInsert.dataInts["data2"] = 4;
            dataToInsert.timeSeconds = 110.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 5.0f;
            dataToInsert.dataFloats["data3"] = 6.0f;
            dataToInsert.timeSeconds = 120.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataInts["data2"] = 7;
            dataToInsert.dataFloats["data3"] = 8.0f;
            dataToInsert.dataFloats["data4"] = 9.0f;
            dataToInsert.timeSeconds = 130.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.timeSeconds = 140.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 10.0f;
            dataToInsert.timeSeconds = 150.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 11.0f;
            dataToInsert.timeSeconds = 145.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        {
            DataToInsert dataToInsert;
            dataToInsert.dataFloats["data1"] = 12.0f;
            dataToInsert.timeSeconds = 160.0f;
            dataHolder.insertData(dataToInsert);
        }
        
        Providers::DataCollector collector;
        auto data = collector.data;
        dataHolder.forEachDataRange(collector);
        
        Providers::DataCollectorData expectedData;
        expectedData.collectedData.insert_or_assign("data1", DataRange<float>{
            .data = {1.0f, 3.0f, 5.0f, 11.0f, 10.0f, 12.0f},
            .timeSeconds = {100.0f, 110.0f, 120.0f, 145.0f, 150.0f, 160.0f}
        });
        expectedData.collectedData.insert_or_assign("data2", DataRange<float>{
            .data = {2.0f, 4.0f, 7.0f},
            .timeSeconds = {100.0f, 110.0f, 130.0f}
        });
        expectedData.collectedData.insert_or_assign("data3", DataRange<float>{
            .data = {6.0f, 8.0f},
            .timeSeconds = {120.0f, 130.0f}
        });
        expectedData.collectedData.insert_or_assign("data4", DataRange<float>{
            .data = {9.0f},
            .timeSeconds = {130.0f}
        });
        EXPECT_EQ(data->collectedData, expectedData.collectedData);
    }
}
