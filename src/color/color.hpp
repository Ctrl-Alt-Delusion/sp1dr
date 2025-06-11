#pragma once

#include "../math/math.hpp"

#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>

namespace COLOR {
typedef MATH::Vec3<int> RGBColor;

constexpr RGBColor WHITE = {255, 255, 255};
constexpr RGBColor BLACK = {0, 0, 0};
constexpr RGBColor RED = {255, 0, 0};
constexpr RGBColor GREEN = {0, 255, 0};
constexpr RGBColor BLUE = {0, 0, 255};
constexpr RGBColor YELLOW = {255, 255, 0};
constexpr RGBColor CYAN = {0, 255, 255};
constexpr RGBColor MAGENTA = {255, 0, 255};
constexpr RGBColor GRAY = {128, 128, 128};

const std::string DEFAULT_ANSI = "\033[0m"; // Reset ANSI color code

static std::string RGB_to_HEX(int r, int g, int b) {
    std::ostringstream oss;
    oss << "#" << std::hex << std::setfill('0') << std::setw(2) << r
        << std::setw(2) << g << std::setw(2) << b;
    return oss.str();
}

static std::string RGB_to_HEX(const std::tuple<int, int, int>& rgb) {
    return RGB_to_HEX(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
}

static std::string RGB_TO_ANSI_STRING(int r, int g, int b) {
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

} // namespace COLOR

