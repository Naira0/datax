#pragma once

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>

namespace dtx
{
    enum class Type
    {
        NUMBER, BOOL, STRING, ARRAY, RECORD
    };

    struct Record;

    // the key-value fields under a header
    using Fields =  std::unordered_map<std::string, Record>;

    using Value = std::variant<double, bool, std::string,
    std::vector<Record>,
    Fields>;

    struct Record
    {
        Value value;
        Type type;
    };

    // all the headers and their fields in a datax file
    using Records = std::unordered_map<std::string, Fields>;
}