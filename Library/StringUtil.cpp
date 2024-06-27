#include "StringUtil.h"

std::string Util::RemoveWhitespace(const char* command) {
  std::string clean(command);
  clean.erase(remove_if(clean.begin(), clean.end(), ::isspace), clean.end());
  return clean;
}
bool Util::IsUpperCase(std::string command){

    for (char c : command) {
        if (!std::isupper(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}


bool Util::IsLowerCase(std::string command){
        for (char c : command) {
        if (!std::islower(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}
