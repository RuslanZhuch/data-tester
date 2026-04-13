#pragma once
#include "data-processors.h"
#include "data-range.h"
#include "data-comparators.h"

#include "gtest/gtest.h"

#include <format>

// Compares two vectors of AcceptedDataElement with float tolerance.
// Uses ASSERT on size so per-element checks don't run with a mismatched vector.
#define EXPECT_ACCEPTED_DATA_ELEMENTS_EQ(actual, expected)                                          \
    do {                                                                                            \
        ASSERT_EQ((actual).size(), (expected).size()) << "acceptedDataElements size mismatch";     \
        for (size_t _ade_i = 0; _ade_i < (actual).size(); ++_ade_i)                               \
        {                                                                                           \
            EXPECT_NEAR((actual)[_ade_i].inPoint1,      (expected)[_ade_i].inPoint1,      0.01f)  \
                << "inPoint1 mismatch at index " << _ade_i;                                        \
            EXPECT_NEAR((actual)[_ade_i].inPoint2,      (expected)[_ade_i].inPoint2,      0.01f)  \
                << "inPoint2 mismatch at index " << _ade_i;                                        \
            EXPECT_NEAR((actual)[_ade_i].inTimeSeconds, (expected)[_ade_i].inTimeSeconds, 0.01f)  \
                << "inTimeSeconds mismatch at index " << _ade_i;                                   \
        }                                                                                           \
    } while (false)

namespace Mocks
{
    class DataProcessorMock : public DataProcessors::DataProcessorBase
    {
    public:
        struct AcceptedDataElement
        {
            float inPoint1{};
            float inPoint2{};
            float inTimeSeconds{};
        };
        
    public:
        DataProcessorMock(std::string inTag)
            : tag(std::move(inTag))
        {
            
        }
        
        void acceptData(float inPoint1, float inPoint2, float inTimeSeconds) noexcept override
        {
            acceptedDataElements.emplace_back(inPoint1, inPoint2, inTimeSeconds);
        }
        
        void extractIssues(issues_t& inOutProblemsData) const noexcept override
        {
            inOutProblemsData += tag;
        }
        
        void onNewDataBlock() noexcept override
        {
            ++numOfNewDataBlocks;
        }
    public:
        int32_t numOfNewDataBlocks{};
        std::vector<AcceptedDataElement> acceptedDataElements;
        
    private:
        std::string tag;
        std::string acceptedData;
    };
    
    class DataComparatorMock : public DataComparators::DataComparatorBase
    {
    public:
        struct Record
        {
            DataRange<float> dataRange1;
            DataRange<float> dataRange2;
            float timeOffsetSeconds{};
            std::string processors;
            
            constexpr auto operator<=>(const Record& inOther) const = default;
        };
    public:
        bool compareData([[maybe_unused]] const DataRange<float>& inDataRange1, [[maybe_unused]] const DataRange<float>& inDataRange2, [[maybe_unused]] const float inTimeOffsetSeconds, [[maybe_unused]] std::span<DataProcessors::DataProcessorBase* const> inDataProcessors) const noexcept override
        {
            
            Record record
            {
                .dataRange1 = inDataRange1,
                .dataRange2 = inDataRange2,
                .timeOffsetSeconds = inTimeOffsetSeconds,
                .processors = {},
            };
            
            for (const DataProcessors::DataProcessorBase* processor : inDataProcessors)
            {
                DataProcessors::DataProcessorBase::issues_t issues;
                processor->extractIssues(issues);
                if (!issues.empty())
                {
                    record.processors += std::format("{}. ", issues);
                }
            }
            
            this->records.push_back(record);
            return true;
        }
    public:
        mutable std::vector<Record> records;
    };
}
