#include "screen.hpp"

namespace CORE {

    ScreenSettings DEFAULT_SETTINGS {60};
    
    // Screen constructor doing initial config 
    Screen::Screen(pair_uint size, ScreenSettings set) {
        this->screen_size = size;
        this->settings = set;

        this->screen_buffer = std::vector<std::vector<char>>(
            this->screen_size.x,
            std::vector<char>(this->screen_size.y, ' ') 
        );
    }

    // Paint an individual pixel if valid position
    bool Screen::set_pixel(pair_uint pos, char rune) {
        if (pos.x >= this->screen_size.x || pos.y >= this->screen_size.y) {
            std::cerr << "invalid position coordinates entered | x : " << pos.x << " | y : " << pos.y << std::endl;
            return false;
        }
        this->screen_buffer[pos.x][pos.y] = rune;
        return true; 
    }

    // Clear screen, + additional config for windows systems
    void Screen::clear_screen() {
        #if defined(_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        #endif

        std::cout << "\033[2J\033[H" << std::flush;
    }

}