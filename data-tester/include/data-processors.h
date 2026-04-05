#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <memory>

namespace DataProcessors
{
    class DataProcessorBase
    {
    public:
        using issues_t = std::string;
    public:
        virtual ~DataProcessorBase() = default;
    public:
        virtual void acceptData(float inPoint1, float inPoint2, float inTimeSeconds) noexcept = 0;
        virtual void onNewDataBlock() noexcept {};
        
        virtual void extractIssues(issues_t& inOutProblemsData) const noexcept = 0;
    };

    class DataProcessorMeanSquaredError : public DataProcessorBase
    {
    public:
        struct Desc
        {
            float maxMeanSquaredError{};
        };
    public:
        DataProcessorMeanSquaredError(const Desc& inDesc) noexcept
            : desc{std::move(inDesc)}
        {
            
        }

        void acceptData(float inPoint1, float inPoint2, float inTimeSeconds) noexcept override;
        void extractIssues(issues_t& inOutProblemsData) const noexcept override;
    private:
        [[nodiscard]] float getMeanSquaredError() const noexcept;
    private:
        float sumOfSquares{};
        int32_t numOfErrors{};
        
        Desc desc{};
    };

    class DataProcessorMaxAbsoluteError : public DataProcessorBase
    {
    public:
        struct Desc
        {
            float maxAbsoluteError{};
        };
    public:
        DataProcessorMaxAbsoluteError(const Desc& inDesc) noexcept
            : desc{std::move(inDesc)}
        {
            
        }
        
//        DataProcessorMaxAbsoluteError(const DataProcessorMaxAbsoluteError& inOther) noexcept
//            : desc{inOther.desc}
//        {
//        }
//
//        DataProcessorMaxAbsoluteError& operator=(const DataProcessorMaxAbsoluteError& inOther) noexcept
//        {
//            if (this != &inOther)
//                *this = DataProcessorMaxAbsoluteError(Desc{inOther.desc});
//            return *this;
//        }
        
        void acceptData(float inPoint1, float inPoint2, float inTimeSeconds) noexcept override;
        void extractIssues(issues_t& inOutProblemsData) const noexcept override;
        
        void onNewDataBlock() noexcept override;
        
    private:
        float currentMaxAbsoluteError{-1.f};
        int32_t maxErrorDataIndex{};
        float maxErrorTime{};
        int32_t currentDataIndex{};
        
        Desc desc{};
    };
}
