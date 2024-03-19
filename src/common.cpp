#include "../include/common.h"

void super_split(const std::string& s, std::vector<std::string>& v, const std::string& c) {
    std::string::size_type pos1, pos2;
    size_t len = s.length();
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.emplace_back(s.substr(pos1, pos2 - pos1));
        pos1 = pos2 + c.size();
    }
    if (pos1 != len) {
        v.emplace_back(s.substr(pos1));
    }
}

