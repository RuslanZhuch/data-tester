#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace DataImporter
{
    struct DataRow
    {
        float time{};
        float dataLeft{};
        float dataRight{};
    };

    struct Table
    {
        std::vector<DataRow> rows;
    };

    // CSV layout (';' delimited, ',' decimal separator):
    //   row 0: processor type headers  (skipped)
    //   row 1: processor threshold values  (skipped)
    //   row 2: table names  (e.g. "data1;;;data2;;")
    //   row 3: column headers  (time;data_left;data_right per table, skipped)
    //   row 4+: data rows  (sparse — a table's columns may be empty for a given row)
    [[nodiscard]] std::unordered_map<std::string, Table> importFromCSV(const std::filesystem::path& inFilePath);
}
