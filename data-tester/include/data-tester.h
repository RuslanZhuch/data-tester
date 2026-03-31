#pragma once

#include <span>

namespace DataProcessors
{
    class DataProcessorBase;
};

namespace DataComparators
{
    class DataComparatorBase;
};

class DataHolder;

namespace DataTester
{

    struct TestRules
    {
        DataComparators::DataComparatorBase* dataComparator{};
        DataProcessors::DataProcessorBase* dataProcessor{};
    };

    void TestData(const DataHolder& inDataHolder, const TestRules& inDefaultTestRules, const std::span<const TestRules> inTestRules);
}
