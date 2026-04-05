#include "data-processors.h"
#include "data-importer.h"

#include <gtest/gtest.h>

#include <string>

namespace
{
    void fillProcessor(const std::string& inEntryName, DataProcessors::DataProcessorBase& inOutProcessor)
    {
        static const std::filesystem::path csvPath = 
            std::filesystem::path(__FILE__).parent_path().parent_path() / "resources/test-data.csv";
        static std::unordered_map<std::string, DataImporter::Table> importedTables = DataImporter::importFromCSV(csvPath);
        
        const auto& entry = importedTables.find(inEntryName);
        if (entry == importedTables.end())
        {
            return;
        }
        
        for (const DataImporter::DataRow& row : entry->second.rows)
        {
            inOutProcessor.acceptData(row.dataLeft, row.dataRight, row.time);
        }
    }
}

namespace DataProcessorsTest
{
    TEST(DataProcessorMeanSquare, Empty)
    {
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError({});
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMeanSquaredError.extractIssues(issues);
        EXPECT_TRUE(issues.empty());
    }

//    TEST(DataProcessorMeanSquare, SameData)
//    {
//        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError;
//
//        dataProcessorMeanSquaredError.acceptData(1.0f, 1.0f, 101.0f);
//        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
//        
//        dataProcessorMeanSquaredError.acceptData(2.0f, 2.0f, 102.0f);
//        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
//        
//        dataProcessorMeanSquaredError.acceptData(10.0f, 10.0f, 103.0f);
//        EXPECT_EQ(dataProcessorMeanSquaredError.getMeanSquaredError(), 0.0f);
//    }
    
    TEST(DataProcessorMeanSquare, Normal)
    {
        const DataProcessors::DataProcessorMeanSquaredError::Desc desc{
            .maxMeanSquaredError = 0.2f,
        };
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError(desc);
        fillProcessor("data1", dataProcessorMeanSquaredError);
        
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMeanSquaredError.extractIssues(issues);
        ASSERT_EQ(issues.size(), 0);
    }
    
    TEST(DataProcessorMeanSquare, CatchError1)
    {
        const DataProcessors::DataProcessorMeanSquaredError::Desc desc{
            .maxMeanSquaredError = 0.05f,
        };
        DataProcessors::DataProcessorMeanSquaredError dataProcessorMeanSquaredError(desc);
        fillProcessor("data1", dataProcessorMeanSquaredError);
        
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMeanSquaredError.extractIssues(issues);
        EXPECT_EQ(issues, R"(Mean squared error 0.1706 is greater than 0.05. )");
    }
    
    TEST(DataProcessorMaxAbsoluteError, Empty)
    {
        DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError({});
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMaxAbsoluteError.extractIssues(issues);
        EXPECT_TRUE(issues.empty());
    }
    
    TEST(DataProcessorMaxAbsoluteError, Normal)
    {
        const DataProcessors::DataProcessorMaxAbsoluteError::Desc desc{
            .maxAbsoluteError = 0.9f,
        };
        DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError(desc);
        fillProcessor("data1", dataProcessorMaxAbsoluteError);
        
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMaxAbsoluteError.extractIssues(issues);
        ASSERT_EQ(issues.size(), 0);
    }
    
    TEST(DataProcessorMaxAbsoluteError, CatchError1)
    {
        const DataProcessors::DataProcessorMaxAbsoluteError::Desc desc{
            .maxAbsoluteError = 0.08f,
        };
        DataProcessors::DataProcessorMaxAbsoluteError dataProcessorMaxAbsoluteError(desc);
        fillProcessor("data1", dataProcessorMaxAbsoluteError);
        
        DataProcessors::DataProcessorBase::issues_t issues;
        dataProcessorMaxAbsoluteError.extractIssues(issues);
        EXPECT_EQ(issues, R"(Absolute error 0.8300 is greater than 0.08, time: 101.0000, dataIndex: 1. )");
    }
}

