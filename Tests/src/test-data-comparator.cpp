#include "data-comparators.h"
#include "data-processors.h"
#include "data-range.h"

#include "mocks.h"

#include "gtest/gtest.h"

namespace DataComparatorTest
{
    TEST(DataComparatorLinear, Empty)
    {
    
        DataRange<float> dataRange1;
        DataRange<float> dataRange2;
        float timeOffsetSeconds = 0.0f;
        
        Mocks::DataProcessorMock dataProcessor1("Processor1");
        Mocks::DataProcessorMock dataProcessor2("Processor2");
        
        std::vector<DataProcessors::DataProcessorBase*> dataProcessors {
            &dataProcessor1,
            &dataProcessor2,
        };

        DataComparators::DataComparatorLinear comparator;
        EXPECT_TRUE(comparator.compareData(dataRange1, dataRange2, timeOffsetSeconds, dataProcessors));
        
        EXPECT_EQ(dataProcessor1.numOfNewDataBlocks, 1);
        EXPECT_EQ(dataProcessor2.numOfNewDataBlocks, 1);
        
        EXPECT_EQ(dataProcessor1.acceptedDataElements.size(), 0);
        EXPECT_EQ(dataProcessor2.acceptedDataElements.size(), 0);
    }
    
    TEST(DataComparatorLinear, Normal)
    {
        DataRange<float> dataRange1{
            .data = {1.f, 4.f, 6.f, 12.f, 16.f},
            .timeSeconds = {10.f, 20.f, 25.f, 30.f, 35.f},
        };
        DataRange<float> dataRange2{
            .data = {50.f, 55.f, 65.f, 75.f, 100.f},
            .timeSeconds = {10.f, 19.f, 25.f, 31.f, 35.f},
        };
        float timeOffsetSeconds = 0.0f;
        
        Mocks::DataProcessorMock dataProcessor1("Processor1");
        Mocks::DataProcessorMock dataProcessor2("Processor2");
        
        std::vector<DataProcessors::DataProcessorBase*> dataProcessors {
            &dataProcessor1,
            &dataProcessor2,
        };

        DataComparators::DataComparatorLinear comparator;
        EXPECT_TRUE(comparator.compareData(dataRange1, dataRange2, timeOffsetSeconds, dataProcessors));
        
        EXPECT_EQ(dataProcessor1.numOfNewDataBlocks, 1);
        EXPECT_EQ(dataProcessor2.numOfNewDataBlocks, 1);
        
        EXPECT_EQ(dataProcessor1.acceptedDataElements.size(), 5);
        EXPECT_EQ(dataProcessor2.acceptedDataElements.size(), 5);
        
        std::vector expectedAcceptedDataElements {
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 1.f,
                .inPoint2 = 50.f,
                .inTimeSeconds = 10.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 4.f,
                .inPoint2 = 56.67f,
                .inTimeSeconds = 20.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 6.f,
                .inPoint2 = 65.f,
                .inTimeSeconds = 25.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 12.f,
                .inPoint2 = 73.33f,
                .inTimeSeconds = 30.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 16.f,
                .inPoint2 = 100.f,
                .inTimeSeconds = 35.f,
            },
        };

        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor1.acceptedDataElements, expectedAcceptedDataElements);
        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor2.acceptedDataElements, expectedAcceptedDataElements);
    }
    
    TEST(DataComparatorLinear, NormalSecondOneIsShorter)
    {
        DataRange<float> dataRange1{
            .data = {6.f, 12.f, 16.f},
            .timeSeconds = {25.f, 30.f, 35.f},
        };
        DataRange<float> dataRange2{
            .data = {65.f, 75.f, 100.f},
            .timeSeconds = {25.f, 31.f, 34.f},
        };
        float timeOffsetSeconds = 0.0f;
        
        Mocks::DataProcessorMock dataProcessor1("Processor1");
        Mocks::DataProcessorMock dataProcessor2("Processor2");
        
        std::vector<DataProcessors::DataProcessorBase*> dataProcessors {
            &dataProcessor1,
            &dataProcessor2,
        };

        DataComparators::DataComparatorLinear comparator;
        EXPECT_TRUE(comparator.compareData(dataRange1, dataRange2, timeOffsetSeconds, dataProcessors));
        
        EXPECT_EQ(dataProcessor1.numOfNewDataBlocks, 1);
        EXPECT_EQ(dataProcessor2.numOfNewDataBlocks, 1);
        
        EXPECT_EQ(dataProcessor1.acceptedDataElements.size(), 3);
        EXPECT_EQ(dataProcessor2.acceptedDataElements.size(), 3);
        
        std::vector expectedAcceptedDataElements {
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 6.f,
                .inPoint2 = 65.f,
                .inTimeSeconds = 25.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 12.f,
                .inPoint2 = 73.33f,
                .inTimeSeconds = 30.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 16.f,
                .inPoint2 = 108.33f,
                .inTimeSeconds = 35.f,
            },
        };

        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor1.acceptedDataElements, expectedAcceptedDataElements);
        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor2.acceptedDataElements, expectedAcceptedDataElements);
    }
    
    TEST(DataComparatorLinear, NormalSecondOneIsLonger)
    {
        DataRange<float> dataRange1{
            .data = {6.f, 12.f, 16.f},
            .timeSeconds = {25.f, 30.f, 35.f},
        };
        DataRange<float> dataRange2{
            .data = {65.f, 75.f, 100.f},
            .timeSeconds = {25.f, 31.f, 36.f},
        };
        float timeOffsetSeconds = 0.0f;
        
        Mocks::DataProcessorMock dataProcessor1("Processor1");
        Mocks::DataProcessorMock dataProcessor2("Processor2");
        
        std::vector<DataProcessors::DataProcessorBase*> dataProcessors {
            &dataProcessor1,
            &dataProcessor2,
        };

        DataComparators::DataComparatorLinear comparator;
        EXPECT_TRUE(comparator.compareData(dataRange1, dataRange2, timeOffsetSeconds, dataProcessors));
        
        EXPECT_EQ(dataProcessor1.numOfNewDataBlocks, 1);
        EXPECT_EQ(dataProcessor2.numOfNewDataBlocks, 1);
        
        EXPECT_EQ(dataProcessor1.acceptedDataElements.size(), 3);
        EXPECT_EQ(dataProcessor2.acceptedDataElements.size(), 3);
        
        std::vector expectedAcceptedDataElements {
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 6.f,
                .inPoint2 = 65.f,
                .inTimeSeconds = 25.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 12.f,
                .inPoint2 = 73.33f,
                .inTimeSeconds = 30.f,
            },
            Mocks::DataProcessorMock::AcceptedDataElement {
                .inPoint1 = 16.f,
                .inPoint2 = 95.f,
                .inTimeSeconds = 35.f,
            },
        };

        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor1.acceptedDataElements, expectedAcceptedDataElements);
        EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(dataProcessor2.acceptedDataElements, expectedAcceptedDataElements);
    }
}