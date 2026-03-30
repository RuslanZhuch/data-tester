#include <gtest/gtest.h>
#include "data-processors.h"

#include <string>

namespace DataProcessorsTest
{
    TEST(DataProcessorMeanSquare, Empty)
    {
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError;
        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
    }

    TEST(DataProcessorMeanSquare, SameData)
    {
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError;

        dataProcessorMeanSquaredError.acceptData(1.0f, 1.0f, 101.0f);
        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
        
        dataProcessorMeanSquaredError.acceptData(2.0f, 2.0f, 102.0f);
        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
        
        dataProcessorMeanSquaredError.acceptData(10.0f, 10.0f, 103.0f);
        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
    }
    
    TEST(DataProcessorMeanSquare, Normal)
    {
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError;

        dataProcessorMeanSquaredError.acceptData(1.0f, 2.0f, 101.0f);
        dataProcessorMeanSquaredError.acceptData(2.0f, 1.2f, 102.0f);
        dataProcessorMeanSquaredError.acceptData(3.0f, 3.1f, 103.0f);
        dataProcessorMeanSquaredError.acceptData(5.0f, 4.f, 104.0f);
        dataProcessorMeanSquaredError.acceptData(8.0f, 8.25f, 105.0f);
        dataProcessorMeanSquaredError.acceptData(8.0f, 8.35f, 106.0f);
        dataProcessorMeanSquaredError.acceptData(7.5f, 7.f, 107.0f);
        dataProcessorMeanSquaredError.acceptData(-1.f, 0.2f, 108.0f);
        dataProcessorMeanSquaredError.acceptData(-2.f, 2.f, 109.0f);

        EXPECT_NEAR(dataProcessorMeanSquaredError.getMeanSquaredError(), 2.2806f, 0.0001f);
        EXPECT_NEAR(dataProcessorMeanSquaredError.getMeanSquaredError(), 2.2806f, 0.0001f);

        dataProcessorMeanSquaredError.onNewDataBlock();
        EXPECT_NEAR(dataProcessorMeanSquaredError.getMeanSquaredError(), 2.2806f, 0.0001f);
    }
    
    TEST(DataProcessorMaxAbsoluteError, Empty)
    {
        DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
        EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError(), std::nullopt);
    }

    TEST(DataProcessorMaxAbsoluteError, OneDataPoint)
    {
        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(1.0f, 10.0f, 101.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError(), 9.0f);
        }

        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(5.0f, 2.0f, 101.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError(), 3.0f);
        }
        
        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 20.0f, 101.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError(), 0.0f);
        }
    }

    TEST(DataProcessorMaxAbsoluteError, Normal)
    {
        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 20.0f, 101.0f);
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 30.0f, 102.0f);
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 15.0f, 103.0f);
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 21.0f, 104.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError().value_or(0.0f), 10.0f);
        }
        
        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(20.0f, 20.0f, 101.0f);
            dataProcessorMaxAbsoluteError.acceptData(30.0f, 20.0f, 102.0f);
            dataProcessorMaxAbsoluteError.acceptData(15.0f, 20.0f, 103.0f);
            dataProcessorMaxAbsoluteError.acceptData(21.0f, 20.0f, 104.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError().value_or(0.0f), 10.0f);
        }
        
        
        {
            DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError;
            dataProcessorMaxAbsoluteError.acceptData(1.0f, 1.0f, 101.0f);
            dataProcessorMaxAbsoluteError.acceptData(2.0f, 2.0f, 102.0f);
            dataProcessorMaxAbsoluteError.acceptData(3.0f, 3.0f, 103.0f);
            dataProcessorMaxAbsoluteError.acceptData(4.0f, 4.0f, 104.0f);
            EXPECT_EQ(dataProcessorMaxAbsoluteError.getMaxAbsoluteError().value_or(42.f), 0.0f);
        }
    }
}

