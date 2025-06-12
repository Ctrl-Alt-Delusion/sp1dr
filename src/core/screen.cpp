#include "screen.hpp"

namespace CORE {

    ScreenSettings DEFAULT_SETTINGS {60};

    // Screen constructor doing initial config 
    Screen::Screen(Vec2Int size, ScreenSettings set) {
        this->screen_size = size;
        this->settings = set;

        this->screen_buffer = std::vector<std::vector<char>>(
            this->screen_size.y,
            std::vector<char>(this->screen_size.x, ' ') 
        );

        this->color_buffer = std::vector<std::vector<COLOR::RGBColor>>(
            this->screen_size.y,
            std::vector<COLOR::RGBColor>(this->screen_size.x, COLOR::RGBColor{0, 0, 0})
        );
    }

    // Paint an individual pixel if valid position
    bool Screen::set_pixel(Vec2Int pos, char rune) {
        if (pos.x >= this->screen_size.x || pos.y >= this->screen_size.y || pos.x < 0 || pos.y < 0) {
            std::cerr << "invalid position coordinates entered | x : " << pos.x << " | y : " << pos.y << std::endl;
            return false;
        }
        this->screen_buffer[pos.y][pos.x] = rune;
        change = 1;
        return true; 
    }

    bool Screen::set_pixel_color(Vec2Int pos, const COLOR::RGBColor& color) {
        if (pos.x >= this->screen_size.x || pos.y >= this->screen_size.y || pos.x < 0 || pos.y < 0) {
            std::cerr << "invalid position coordinates entered | x : " << pos.x << " | y : " << pos.y << std::endl;
            return false;
        }
        if (static_cast<int>(color_buffer.size()) <= pos.y) {
            color_buffer.resize(this->screen_size.y, std::vector<COLOR::RGBColor>(this->screen_size.x));
        }
        this->color_buffer[pos.y][pos.x] = color;
        return true; 
    }

    // Clear screen, + additional config for windows systems
    void Screen::clear_screen() {
        #if defined(_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD topLeft = {0, 0};
            SetConsoleCursorPosition(hOut, topLeft);
        #else
            std::cout << "\033[2J\033[H";
        #endif
    }


}