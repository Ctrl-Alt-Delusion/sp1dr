#pragma once
#include "../math/math.hpp"
#include <string>
#include <sstream>
#include <iomanip>

namespace COLOR {
    // Use Vec3 for RGB colors
    typedef MATH::Vec3<int> RGBColor;
    
    // Constructor helper for RGBColor
    inline RGBColor make_rgb(int r, int g, int b) {
        RGBColor color;
        color.x = r;
        color.y = g;
        color.z = b;
        return color;
    }
    
    // Basic color constants
    const RGBColor WHITE = make_rgb(255, 255, 255);
    const RGBColor BLACK = make_rgb(0, 0, 0);
    const RGBColor RED = make_rgb(255, 0, 0);
    const RGBColor GREEN = make_rgb(0, 255, 0);
    const RGBColor BLUE = make_rgb(0, 0, 255);
    const RGBColor YELLOW = make_rgb(255, 255, 0);
    const RGBColor CYAN = make_rgb(0, 255, 255);
    const RGBColor MAGENTA = make_rgb(255, 0, 255);
    const RGBColor GRAY = make_rgb(128, 128, 128);
    
    // Convert RGB to hex string
    static std::string to_hex(const RGBColor& color) {
        std::ostringstream oss;
        oss << "#" << std::hex << std::setfill('0') 
            << std::setw(2) << color.x
            << std::setw(2) << color.y 
            << std::setw(2) << color.z;
        return oss.str();
    }
    
    // Simple color mixing
    static RGBColor mix(const RGBColor& a, const RGBColor& b, float ratio = 0.5f) {
        if (ratio < 0.0f) ratio = 0.0f;
        if (ratio > 1.0f) ratio = 1.0f;
        
        return make_rgb(
            static_cast<int>(a.x + ratio * (b.x - a.x)),
            static_cast<int>(a.y + ratio * (b.y - a.y)),
            static_cast<int>(a.z + ratio * (b.z - a.z))
        );
    }

    inline std::string RGB_TO_ANSI_STRING(int r, int g, int b) {
        std::ostringstream oss;
        oss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
        return oss.str();
    }

    const std::string ANSI_RESET = "\033[0m";
    
} // namespace COLOR