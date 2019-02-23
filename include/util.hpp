#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <iostream>
#include <sstream>

template<typename T>
std::string ToString(const T &val) {
    std::stringstream ss;
    ss << val;
    return ss.str();
}

#endif // _UTIL_HPP
