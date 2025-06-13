#pragma once

#include "component.hpp"
#include "../utils/math.hpp"

#include <string>
#include <iomanip>
#include <sstream>

namespace COMPONENT {

    typedef MATH::Vec3<int> RGBColor;


    const RGBColor WHITE = make_rgb(255, 255, 255);
    const RGBColor BLACK = make_rgb(0, 0, 0);
    const RGBColor RED = make_rgb(255, 0, 0);
    const RGBColor GREEN = make_rgb(0, 255, 0);
    const RGBColor BLUE = make_rgb(0, 0, 255);
    const RGBColor YELLOW = make_rgb(255, 255, 0);
    const RGBColor CYAN = make_rgb(0, 255, 255);
    const RGBColor MAGENTA = make_rgb(255, 0, 255);
    const RGBColor GRAY = make_rgb(128, 128, 128);


    class Color : public Component {
    private:
        RGBColor color_rgb {};
    public:

        explicit Color(RGBColor color_vals) : color_rgb(color_vals) {};

        static std::string to_hex(const RGBColor& color) {
            std::ostringstream oss;
            oss << "#" << std::hex << std::setfill('0') 
                << std::setw(2) << color.x
                << std::setw(2) << color.y 
                << std::setw(2) << color.z;
            return oss.str();
        }


    };
}