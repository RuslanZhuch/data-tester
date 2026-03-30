#include "data-extractor.h"

std::optional<DataExtractor::TimeRange> DataExtractor::getTimeRange([[maybe_unused]] const DataRangeFloat& inDataRange) noexcept
{
    if (inDataRange.timeSeconds.empty())
    {
        return std::nullopt;
    }
    return DataExtractor::TimeRange{
        .startSeconds = inDataRange.timeSeconds.front(),
        .endSeconds = inDataRange.timeSeconds.back()
    };
}

std::optional<DataExtractor::TimeRange> DataExtractor::getTimeRange([[maybe_unused]] const DataRangeInt& inDataRange) noexcept
{
    return {};
}
