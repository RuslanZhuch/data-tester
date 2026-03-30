#include <gtest/gtest.h>
#include "data-extractor.h"

#include <string>

namespace DataExtractorTest
{
    static constexpr DataExtractor::DataRangeView DefaultDataRangeView{};
    TEST(ExtractFromDataFloat, EmptyData)
    {
        DataExtractor::DataRangeFloat dataRangeFloat;
        EXPECT_TRUE(DataExtractor::isEmpty(dataRangeFloat));
        EXPECT_FALSE(DataExtractor::isValid(dataRangeFloat));

        EXPECT_FALSE(DataExtractor::getTimeRange(dataRangeFloat).has_value());

        DataExtractor::DataRangeView dataRangeView;
        EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 0, dataRangeView));
        EXPECT_EQ(dataRangeView, DefaultDataRangeView);

        EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 1, dataRangeView));
        EXPECT_EQ(dataRangeView, DefaultDataRangeView);

        EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 10, dataRangeView));
        EXPECT_EQ(dataRangeView, DefaultDataRangeView);

        EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 100, dataRangeView));
        EXPECT_EQ(dataRangeView, DefaultDataRangeView);
    }

    TEST(ExtractFromDataFloat, OneElementData)
    {
        DataExtractor::DataRangeFloat dataRangeFloat;
        dataRangeFloat.data.push_back(1.0f);
        dataRangeFloat.timeSeconds.push_back(2.0f);

        EXPECT_FALSE(DataExtractor::isEmpty(dataRangeFloat));
        EXPECT_TRUE(DataExtractor::isValid(dataRangeFloat));

        const std::optional<DataExtractor::TimeRange> timeRange = DataExtractor::getTimeRange(dataRangeFloat);
        ASSERT_TRUE(timeRange.has_value());
        EXPECT_EQ(timeRange->startSeconds, 2.0f);
        EXPECT_EQ(timeRange->endSeconds, 2.0f);

        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 0, dataRangeView));
            EXPECT_EQ(dataRangeView, DefaultDataRangeView);
        }

        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 1, dataRangeView));
            constexpr DataExtractor::DataRangeView expectedDataRangeView{
                .firstPointIndex = 0,
                .numOfPoints = 1
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);

            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 1, dataRangeView));
            EXPECT_EQ(dataRangeView, expectedDataRangeView);
        }
        
        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 2, dataRangeView));
            EXPECT_EQ(dataRangeView, DefaultDataRangeView);
        }

        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 10, dataRangeView));
            EXPECT_EQ(dataRangeView, DefaultDataRangeView);
        }
    }
    
    TEST(ExtractFromDataFloat, ElementDataNormal)
    {
        DataExtractor::DataRangeFloat dataRangeFloat;
        dataRangeFloat.data.push_back(1.0f);
        dataRangeFloat.data.push_back(2.0f);
        dataRangeFloat.data.push_back(3.0f);
        dataRangeFloat.data.push_back(4.0f);
        dataRangeFloat.data.push_back(5.0f);
        dataRangeFloat.data.push_back(6.0f);
        dataRangeFloat.data.push_back(7.0f);
        dataRangeFloat.timeSeconds.push_back(101.0f);
        dataRangeFloat.timeSeconds.push_back(102.0f);
        dataRangeFloat.timeSeconds.push_back(103.0f);
        dataRangeFloat.timeSeconds.push_back(104.0f);
        dataRangeFloat.timeSeconds.push_back(105.0f);
        dataRangeFloat.timeSeconds.push_back(106.0f);
        dataRangeFloat.timeSeconds.push_back(107.0f);

        EXPECT_FALSE(DataExtractor::isEmpty(dataRangeFloat));
        EXPECT_TRUE(DataExtractor::isValid(dataRangeFloat));

        const std::optional<DataExtractor::TimeRange> timeRange = DataExtractor::getTimeRange(dataRangeFloat);
        ASSERT_TRUE(timeRange.has_value());
        EXPECT_EQ(timeRange->startSeconds, 101.0f);
        EXPECT_EQ(timeRange->endSeconds, 107.0f);

        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 0, dataRangeView));
            EXPECT_EQ(dataRangeView, DefaultDataRangeView);
        }

        {
            DataExtractor::DataRangeView dataRangeView;
            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            DataExtractor::DataRangeView expectedDataRangeView{
                .firstPointIndex = 0,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);

            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            expectedDataRangeView = {
                .firstPointIndex = 1,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);
            
            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            expectedDataRangeView = {
                .firstPointIndex = 2,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);

            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            expectedDataRangeView = {
                .firstPointIndex = 3,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);

            EXPECT_TRUE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            expectedDataRangeView = {
                .firstPointIndex = 4,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);
            
            // We cannot extract next 3 points, because there are only 2 points left
            EXPECT_FALSE(DataExtractor::extractNextDataPoints(dataRangeFloat, 3, dataRangeView));
            expectedDataRangeView = {
                .firstPointIndex = 4,
                .numOfPoints = 3
            };
            EXPECT_EQ(dataRangeView, expectedDataRangeView);
        }
    }

}
