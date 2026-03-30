#pragma once

#include <cstdint>
#include <optional>
namespace DataProcessors
{
    class DataProcessorBase
    {
    public:
        virtual ~DataProcessorBase() = default;
    public:
        virtual void acceptData(const float inPoint1, const float inPoint2, const float inTimeSeconds) noexcept = 0;
        virtual void onNewDataBlock() noexcept {};
    };

    class DataProcessorMeanSquaredError : public DataProcessorBase
    {
    public:
        void acceptData(const float inPoint1, const float inPoint2, const float inTimeSeconds) noexcept override;
        [[nodiscard]] float getMeanSquaredError() const noexcept;

    private:
        float sumOfSquares{};
        int32_t numOfErrors{};
    };

    class DataProcessorMaxAbsoluteError : public DataProcessorBase
    {
    public:
        void acceptData(const float inPoint1, const float inPoint2, const float inTimeSeconds) noexcept override;
        [[nodiscard]] std::optional<float> getMaxAbsoluteError() const noexcept;
    private:
        float maxAbsoluteError{-1.f};
    };
}
