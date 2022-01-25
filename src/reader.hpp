#pragma once

#include <string>
#include <utility>

#include "types.hpp"

namespace dtx
{

    enum class From
    {
        STRING,
        FILE
    };

    class Reader
    {
    public:
        Reader(From from, const std::string& str);

        struct State
        {
            bool ok = true;
            const char *message;
        } state;

        Records records;

    private:
        std::string source;

        Fields current_record;
        std::string current_key;
        std::string current_record_name;

        size_t offset = 0;
        size_t start  = 0;

        // the position of the last terminator which can include colons
        // mostly used for getting sub strings
        size_t last_terminator = 0;

        void parse();
        void parse_segment(Fields& fields, bool is_nested);

        inline bool at_end();
        inline bool is_terminator(char c);
        inline char advance();
        inline char peek();

        // first is if the value is a bool and second is the value of the bool
        inline std::pair<bool, bool> is_bool();

        inline void set_error(const char *message);

        void scan_header();
        void scan_key();
        void scan_comment();

        Record scan_number();
        Record scan_string();
        Record scan_object();
        Record scan_array();
    };

}


