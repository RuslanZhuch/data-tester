#include "data-tester.h"

#include "data-comparators.h"
#include "data-holder.h"
#include "mocks.h"

#include <gtest/gtest.h>
#include <format>

namespace DataTesterTest
{
    TEST(DataTester, Empty)
    {
        DataHolder dataLeft;
        DataHolder dataRight;
        
        DataToInsert data;

        data.dataFloats["data_1"] = 1.f;
        data.dataFloats["data_2"] = 2.f;
        data.dataFloats["data_special_1"] = 3.f;
        data.dataFloats["data_special_2"] = 4.f;
        data.timeSeconds = 100.f;
        dataLeft.insertData(data);
        
        data.dataFloats["data_1"] = 5.f;
        data.dataFloats["data_2"] = 6.f;
        data.dataFloats["data_special_1"] = 7.f;
        data.dataFloats["data_special_2"] = 8.f;
        data.timeSeconds = 101.f;
        dataLeft.insertData(data);
        
        data.dataFloats["data_1"] = 9.f;
        data.dataFloats["data_2"] = 10.f;
        data.dataFloats["data_special_1"] = 11.f;
        data.dataFloats["data_special_2"] = 12.f;
        data.timeSeconds = 102.f;
        dataRight.insertData(data);
        
        data.dataFloats["data_1"] = 13.f;
        data.dataFloats["data_2"] = 14.f;
        data.dataFloats["data_special_1"] = 15.f;
        data.dataFloats["data_special_2"] = 16.f;
        data.timeSeconds = 103.f;
        dataRight.insertData(data);
        
        Mocks::DataProcessorMock dataProcessor1("processor1");
        Mocks::DataProcessorMock dataProcessor2("processor2");
        Mocks::DataProcessorMock dataProcessor3("processor3");
        Mocks::DataProcessorMock dataProcessor4("processor4");
        Mocks::DataProcessorMock dataProcessor5("processor5");
        
        Mocks::DataComparatorMock dataComparator1;
        Mocks::DataComparatorMock dataComparator2;
        Mocks::DataComparatorMock dataComparator3;

        DataTester::TestRules defaultRules
        {
            .dataComparatorProto = &dataComparator1,
            .dataProcessorsProto = {&dataProcessor1, &dataProcessor2},
        };
        
        DataTester::TestRules specialRules1
        {
            .dataComparatorProto = &dataComparator2,
            .dataProcessorsProto = {&dataProcessor3},
        };
        
        DataTester::TestRules specialRules2
        {
            .dataComparatorProto = &dataComparator3,
            .dataProcessorsProto = {&dataProcessor4, &dataProcessor5},
        };
        
        std::unordered_map<std::string, DataTester::TestRules> specialRules;
        specialRules["data_special_1"] = specialRules1;
        specialRules["data_special_2"] = specialRules2;
        specialRules["data_special_3"] = specialRules1;
        
        DataComparators::CompareSettings compareSettings
        {
            .maxTimeDifferenceSeconds = 10.f,
        };
        const DataTester::TestResult results = DataTester::TestData(dataLeft, dataRight, defaultRules, compareSettings, specialRules);

        std::vector<Mocks::DataComparatorMock::Record> expectedDefaultRecords
        {
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{1.f, 5.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{9.f, 13.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .timeOffsetSeconds = -2,
                .processors = "processor1. processor2. ",
            },
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{9.f, 13.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{1.f, 5.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .timeOffsetSeconds = 2,
                .processors = "processor1. processor2. ",
            },
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{2.f, 6.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{10.f, 14.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .timeOffsetSeconds = -2,
                .processors = "processor1. processor2. ",
            },
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{10.f, 14.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{2.f, 6.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .timeOffsetSeconds = 2,
                .processors = "processor1. processor2. ",
            },
        };
        
        
        std::vector<Mocks::DataComparatorMock::Record> expectedSpecialRecords1
        {
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{3.f, 7.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{11.f, 15.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .timeOffsetSeconds = -2,
                .processors = "processor3. ",
            },
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{11.f, 15.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{3.f, 7.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .timeOffsetSeconds = 2,
                .processors = "processor3. ",
            },
        };
        
        
        std::vector<Mocks::DataComparatorMock::Record> expectedSpecialRecords2
        {
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{4.f, 8.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{12.f, 16.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .timeOffsetSeconds = -2,
                .processors = "processor4. processor5. ",
            },
            Mocks::DataComparatorMock::Record{
                .dataRange1 = DataRange<float>{
                    .data = {{12.f, 16.f}},
                    .timeSeconds = {{102.f, 103.f}},
                },
                .dataRange2 = DataRange<float>{
                    .data = {{4.f, 8.f}},
                    .timeSeconds = {{100.f, 101.f}},
                },
                .timeOffsetSeconds = 2,
                .processors = "processor4. processor5. ",
            },
        };
        
        
        EXPECT_EQ(dataComparator1.records, expectedDefaultRecords);
        EXPECT_EQ(dataComparator2.records, expectedSpecialRecords1);
        EXPECT_EQ(dataComparator3.records, expectedSpecialRecords2);

    }
}
