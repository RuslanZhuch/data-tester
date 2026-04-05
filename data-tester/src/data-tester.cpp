#include "data-comparators.h"
#include "data-holder.h"
#include "data-tester.h"
#include "data-extractor.h"
#include <memory>
#include <string>
#include <format>

namespace DataTester
{
    TestResult TestData(
        const DataHolder& inDataHolder1,
        const DataHolder& inDataHolder2,
        const TestRules& inDefaultTestRulesProto, 
        const DataComparators::CompareSettings& inCompareSettings,
        const std::unordered_map<std::string, TestRules>& inTestRules
    )
    {
        DataIterator dataIterator;
        
        TestResult result;
        
//        std::vector<std::string> formattedIssuesList;
        
        const auto gatherTestRules = [&](const std::string& inDataName) -> const TestRules&
        {
            const auto it = inTestRules.find(inDataName);
            if (it != inTestRules.end())
            {
                return it->second;
            }
            
            return inDefaultTestRulesProto;
        };
        
        const auto processing = [&gatherTestRules, &result](const std::string& inDataName, const DataExtractor::DataRangeFloat& inDataRange1, const DataComparators::CompareSettings& inCompareSettings, const DataExtractor::DataRangeFloat& inDataRange2)
        {
            const TestRules& actualTestRules = gatherTestRules(inDataName);
            
            const bool bSuccessful = DataComparators::compareData(
                inDataRange1, 
                inDataRange2, 
                inCompareSettings, 
                *actualTestRules.dataComparatorProto, 
                actualTestRules.dataProcessorsProto
            );
            
            if (!bSuccessful)
            {
                return;
            }
            
            DataProcessors::DataProcessorBase::issues_t issues;
            for (const auto& processor : actualTestRules.dataProcessorsProto)
            {
                processor->extractIssues(issues);
            }
            
            if (!issues.empty())
            {
                if (!result.errorJson.empty())
                {
                    result.errorJson.append(",");
                }
                result.errorJson.append(std::format(R"({{"entry": {}, "desc": {}}})", inDataName, issues));
            }
        };
        dataIterator.forEachDataRange(inDataHolder1, inDataHolder2, inCompareSettings, processing);
        
//        for (size_t problemIndex = 0; const std::string& problem : problems)
//        {
//            if (problemIndex > 0)
//            {
//                result.errorJson.append(std::format(", {}", problem));
//            }
//            else
//            {
//                result.errorJson.append(problem);
//            }
//        }
        
        return result;
    }
}
