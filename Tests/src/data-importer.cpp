#include "data-importer.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace DataImporter
{
    namespace detail
    {
        std::vector<std::string> splitRow(const std::string& inRow, char inDelimiter)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream stream(inRow);
            while (std::getline(stream, token, inDelimiter))
            {
                tokens.push_back(token);
            }
            return tokens;
        }

        float parseFloat(const std::string& inStr)
        {
            std::string normalized = inStr;
            std::replace(normalized.begin(), normalized.end(), ',', '.');
            return std::stof(normalized);
        }
    }

    std::unordered_map<std::string, Table> importFromCSV(const std::filesystem::path& inFilePath)
    {
        std::unordered_map<std::string, Table> result;

        std::ifstream file(inFilePath);
        if (!file.is_open())
        {
            return result;
        }

        struct TableInfo
        {
            int32_t startColumn{};
            std::string name;
        };
        std::vector<TableInfo> tables;

        std::string line;
        int32_t rowIndex = 0;
        while (std::getline(file, line))
        {
            const auto tokens = detail::splitRow(line, ';');

            if (rowIndex == 0 || rowIndex == 1)
            {
                // processor config rows — skipped
            }
            else if (rowIndex == 2)
            {
                for (int32_t i = 0; i < std::ssize(tokens); ++i)
                {
                    if (!tokens[i].empty())
                    {
                        tables.push_back({
                            .startColumn =  i,
                            .name = tokens[i], 
                        });
                        result.emplace(tokens[i], Table{});
                    }
                }
            }
            else if (rowIndex == 3)
            {
                // column headers — skipped
            }
            else
            {
                for (const TableInfo& table : tables)
                {
                    const int32_t timeCol  = table.startColumn;
                    const int32_t leftCol  = table.startColumn + 1;
                    const int32_t rightCol = table.startColumn + 2;

                    if (rightCol >= std::ssize(tokens))
                    {
                        continue;
                    }

                    if (tokens[timeCol].empty() || tokens[leftCol].empty() || tokens[rightCol].empty())
                    {
                        continue;
                    }

                    result.at(table.name).rows.push_back({
                        detail::parseFloat(tokens[timeCol]),
                        detail::parseFloat(tokens[leftCol]),
                        detail::parseFloat(tokens[rightCol])
                    });
                }
            }

            ++rowIndex;
        }

        return result;
    }
}
