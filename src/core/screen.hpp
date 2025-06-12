#pragma once

#include "../math/math.hpp"
#include "../color/color.hpp"

#include <iostream>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace CORE {
    using Vec2Int = MATH::Vec2<int>;

    // ScreenSettings in the future will hold
    // all relevant screen config settings
    struct ScreenSettings {
        size_t frameRate;
    };

    extern ScreenSettings DEFAULT_SETTINGS;


    // Screen defines all functionality pertaining
    // to the literal screen shown in every frame
    // and the ability to paint the screen with pixels
    class Screen {
    public:
        Screen(Vec2Int screen_size = {80, 45}, ScreenSettings settings = DEFAULT_SETTINGS);
        ~Screen() = default;

        void clear_screen();
        bool set_pixel(Vec2Int position, char rune);
        bool set_pixel_color(Vec2Int position, const COLOR::RGBColor& color);

        const std::vector<std::vector<char>>& get_screen_buffer() const {
            return this->screen_buffer;
        }

        const std::vector<std::vector<COLOR::RGBColor>>& get_color_buffer() const {
            return this->color_buffer;
        }

        bool get_change() const {
            return change;
        }

        const Vec2Int get_size() {
            return this->screen_size;
        } 

        void set_change(bool c) {
            change = c;
        }
    private:
        Vec2Int        screen_size     {};
        ScreenSettings settings        {};

        std::vector<std::vector<char>> screen_buffer {};
        std::vector<std::vector<COLOR::RGBColor>> color_buffer  {};
        bool change {};
    };


} // CORE