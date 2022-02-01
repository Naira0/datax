#pragma once

#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>

namespace dtx
{
    enum Type : uint8_t
    {
        Number, Bool, String, Array, Object
    };

    struct Record;

    using Fields =  std::unordered_map<std::string, Record>;

    using Value = std::variant<double, bool, std::string,
    std::vector<Record>,
    Fields>;

    using Records = std::unordered_map<std::string, Fields>;

    struct Record
    {
        Value value;

        Record(Value& value) : value(std::move(value)) {}
        Record(Value value) : value(std::move(value)) {}

        Record() = default;

        Record& operator=(Value val)
        {
            value = std::move(val);
            return *this;
        }

        template<size_t I>
        auto& get()
        {
            return std::get<I>(value);
        }

        template<size_t I>
        auto get() const
        {
            return std::get<I>(value);
        }

    };
}