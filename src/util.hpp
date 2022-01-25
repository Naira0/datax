#pragma once

#include <utility>
#include "types.hpp"

namespace dtx
{
    // small utility function for getting a record value
    template<typename T>
    std::pair<T, bool> get_record(dtx::Fields& fields, const std::string& name)
    {
        if(!fields.contains(name))
            return {T(), false};
        dtx::Record record = fields.at(name);
        return {std::get<T>(record.value), true};
    }
}