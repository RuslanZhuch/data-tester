#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace DataProcessors
{
    class DataProcessorBase;
};

namespace DataComparators
{
    class DataComparatorBase;
    struct CompareSettings;
};

class DataHolder;

namespace DataTester
{
    struct TestRules
    {
        const DataComparators::DataComparatorBase* dataComparatorProto{};
        std::vector<DataProcessors::DataProcessorBase*> dataProcessorsProto{};
    };
    
    struct TestResult
    {
        std::string errorJson;
    };

    [[nodiscard]] TestResult TestData(
        const DataHolder& inDataHolder1,
        const DataHolder& inDataHolder2,
        const TestRules& inDefaultTestRulesProto, 
        const DataComparators::CompareSettings& inCompareSettings,
        const std::unordered_map<std::string, TestRules>& inTestRules
    );
}
