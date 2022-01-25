#pragma once

#include <ostream>

#include "types.hpp"

namespace dtx
{
    class Writer
    {
    public:

        Writer(Records& records) : records(records) {}

        // converts datax records to a string
        std::string to_string();

        // converts a datax value to a string
        std::string to_string(dtx::Value& value);

        // to_string should be preferred over to_stream where ever speed is a concern.
        void to_stream(std::ostream& os);

        // returns true if it successfully wrote to the file
        bool write(const char *filename);

    private:
        dtx::Records& records;
    };
}