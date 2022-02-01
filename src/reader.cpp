#include <fstream>
#include <cctype>
#include <sstream>
#include "reader.hpp"

dtx::Reader::Reader(dtx::From from, std::string_view str)
{
    if(from == From::File)
    {
        std::fstream file(str.data());

        if (!file.is_open())
        {
            set_error("could not find specified file");
            return;
        }

        source = std::string((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    }
    else
        source = str;

    parse();
}

void dtx::Reader::parse()
{
    while (!at_end() && state.ok)
    {
        start = offset;
        parse_segment(current_record, false);
    }

    if (!state.ok)
        return;
    if (current_record.empty())
        return set_error("malformed records found");

    this->operator[](current_record_name) = std::move(current_record);
}

void dtx::Reader::parse_segment(Fields& fields, bool is_nested)
{
    char c = advance();

    switch(c)
    {
        case ' ':
        case '\r':
        case '\n':
        case '\t': last_terminator = offset; return;
        case ':': scan_key(); last_terminator = offset; break;
        case '#': scan_comment(); break;
        case '"': fields[current_key] = scan_string(); break;
        case '[': fields[current_key] = scan_array(); break;
        case '{': fields[current_key] = scan_object(); break;
        case '<':
        {
            if (is_nested)
                return set_error("invalid character found within nested value");
            scan_header();
            break;
        }
        default:
        {
            if (offset-1 == last_terminator && std::isdigit(c))
                fields[current_key] = scan_number();
            if (auto [is_true, v] = is_bool(); is_true)
                fields[current_key] = Record(v);
        }
    }
}

bool dtx::Reader::at_end()
{
    return offset >= source.size();
}

bool dtx::Reader::is_terminator(char c)
{
    return
           c == '\n'
        || c == '\r'
        || c == '\t'
        || c == '\0';
}

std::pair<bool, bool> dtx::Reader::is_bool()
{
    auto s = source.substr(last_terminator, offset-last_terminator);
    bool t = s == "true";
    bool f = s == "false";
    return {t || f, t};
}

char dtx::Reader::advance()
{
    if (at_end())
        return '\0';
    return source[offset++];
}

char dtx::Reader::peek()
{
    if (at_end())
        return '\0';
    return source[offset];
}

void dtx::Reader::set_error(const char *message)
{
    state.ok = false;
    state.message = message;
}

void dtx::Reader::scan_header()
{
    if (!current_record.empty())
    {
        this->operator[](current_record_name) = std::move(current_record);
    }

    while (!at_end() && peek() != '>')
        offset++;

    if (at_end())
        return set_error("no fields found for header");

    current_record_name = std::move(source.substr(start+1, offset-start-1));

    current_record = Fields();
}

void dtx::Reader::scan_key()
{
    current_key = std::move(source.substr(last_terminator, offset-last_terminator-1));
}

dtx::Value dtx::Reader::scan_number()
{
    loop: while (!at_end() && isdigit(peek()))
        offset++;

    if (peek() == '.')
    {
        offset++;
        goto loop;
    }


    std::string value = std::move(source.substr(start, offset-start));

    return std::stod(value);
}

dtx::Value dtx::Reader::scan_string()
{
    while(!at_end() && peek() != '"')
        offset++;

    std::string value = std::move(source.substr(start+1, offset-start-1));

    offset++;

    return value;
}

dtx::Value dtx::Reader::scan_array()
{
    start++;

    std::string key_name = std::move(current_key);
    std::vector<Record> values;
    Value val;

    while (!at_end() && peek() != ']')
    {
        char c = advance();
        start = offset-1;

        switch (c)
        {
            case ' ':
            case '\r':
            case '\n':
            case '\t': last_terminator = offset; break;
            case '"': val = scan_string(); break;
            case ',': values.push_back(std::move(val)); break;
            case '{': val = scan_object(); break;
            default:
                if (std::isdigit(c))
                    val = scan_number();
                if (auto [is_true, v] = is_bool(); is_true)
                    val = v;
        }
    }

    values.push_back(std::move(val)); // pushes the last value

    current_key =  std::move(key_name);

    return values;
}

void dtx::Reader::scan_comment()
{
    while(!at_end() && !is_terminator(peek()))
        offset++;
}

dtx::Value dtx::Reader::scan_object()
{
    std::string key_name = std::move(current_key);
    Fields fields;

    last_terminator++; // the key scanning function relies on terminator location

    while (!at_end() && peek() != '}')
    {
        start = offset;
        parse_segment(fields, true);
    }

    current_key = std::move(key_name);

    return fields;
}