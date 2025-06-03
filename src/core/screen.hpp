#pragma once

#include "../math/math.hpp"

#include <iostream>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#endif

typedef MATH::Vec2<size_t> pair_uint;

namespace CORE {

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
        Screen(pair_uint screen_size = {80, 45}, ScreenSettings settings = DEFAULT_SETTINGS);
        ~Screen() = default;

        void clear_screen();
        bool set_pixel(pair_uint position, char rune);

        const std::vector<std::vector<char>>& get_screen_buffer() const {
            return this->screen_buffer;
        }

        const bool get_change() const {
            return change;
        }

        const pair_uint get_size() {
            return this->screen_size;
        } 

        void set_change(bool c) {
            change = c;
        }
    private:
        pair_uint      screen_size     {};
        ScreenSettings settings        {};

        std::vector<std::vector<char>> screen_buffer {};

        bool change {};
    };


} // CORE