#pragma once

#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>

namespace COLOR {

static std::string RGB_to_HEX(int r, int g, int b) {
    std::ostringstream oss;
    oss << "#" << std::hex << std::setfill('0') << std::setw(2) << r
        << std::setw(2) << g << std::setw(2) << b;
    return oss.str();
}

static std::string RGB_to_HEX(const std::tuple<int, int, int>& rgb) {
    return RGB_to_HEX(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
}


} // namespace COLOR

