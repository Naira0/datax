#include <fstream>

#include "writer.hpp"


std::string dtx::Writer::to_string()
{
    std::string output;
    for(auto &[k, v] : records)
    {
        output += "\n<";
        output += k;
        output += ">\n";
        for(auto &[_k, _v] : v)
        {
            output += _k;
            output += ": ";
            output += to_string(_v.value);
            output += '\n';
        }
    }
    return output;
}

void dtx::Writer::to_stream(std::ostream &os)
{
    for(auto &[k, v] : records)
    {
        os
            << "\n<"
            << k
            << ">\n";
        for(auto &[_k, _v] : v)
        {
            os
                << _k
                << ": "
                << to_string(_v.value)
                << '\n';
        }
    }
}

std::string dtx::Writer::to_string(dtx::Value &value)
{
    switch(value.index())
    {
        case 0: return std::to_string(std::get<double>(value));
        case 1: return std::get<bool>(value) ? "true" : "false";
        case 2: return std::get<std::string>(value);
        case 3:
        {
            auto array = std::get<std::vector<dtx::Record>>(value);

            std::string output = "[ ";

            for(auto &elem : array)
            {
                output += to_string(elem.value);
                output += &elem == &array.back() ? " ]" : ", ";
            }
            return output;
        }
        case 4:
        {
            auto fields = std::get<dtx::Fields>(value);
            std::string output = "{ ";
            for(auto &[k, v] : fields)
            {
                output += k;
                output += ": ";
                output += to_string(v.value);
                output += ' ';
            }
            output += '}';
            return output;
        }
        default: return "???";
    }
}

bool dtx::Writer::write(const char *filename)
{
    std::ofstream file(filename);

    if(!file.is_open() || file.bad())
        return false;

    to_stream(file);

    return true;
}


