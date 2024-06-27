#pragma once
#include <string>
#include<algorithm>

namespace Util {
    
    std::string RemoveWhitespace(const char* command);
    bool IsUpperCase(std::string command);
    bool IsLowerCase(std::string command);
}