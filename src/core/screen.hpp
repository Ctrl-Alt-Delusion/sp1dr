#pragma once

#include "../math/math.hpp"

#include <vector>

namespace CORE {
    struct ScreenSettings {
        size_t frameRate;
    };

    ScreenSettings DEFAULT {60};

    class Screen {
    public:
        Screen(MATH::Vec2<size_t> screen_size, ScreenSettings settings = DEFAULT);

    private:
        MATH::Vec2<size_t> screen_size {};
        ScreenSettings settings        {};

        std::vector<std::vector<char>> screen_buffer {};

        void clear_screen();
        void set_pixel(MATH::Vec2<size_t> position, char rune);
    };

} // CORE