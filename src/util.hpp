#pragma once

#include <optional>
#include "types.hpp"

namespace dtx
{
    // small utility function for getting a record value as an optional
    template<typename T>
    std::optional<T> get_record(dtx::Fields& fields, const std::string& name)
    {
        if(!fields.contains(name))
            return std::nullopt;

        dtx::Record record = fields.at(name);

        if(!holds_alternative<T>(record.value))
            return std::nullopt;

        return std::optional(std::get<T>(record.value));
    }
}