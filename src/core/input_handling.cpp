#include "input_handling.hpp"

namespace CORE {
    // ==== PLATFORM-SPECIFIC: Input Stuff ====
    #if defined(_WIN32)
    #include <windows.h>
    char pressed_key = 0;

    bool kbhit() {
        char ch = 0;
        for (char i = 'A'; i <= 'Z'; ++i) {
            if (GetAsyncKeyState(i) & 0x8000) {
                ch = tolower(i);
                break;
            }
        }
        
        if (ch != 0) {
            pressed_key = ch; // Store the pressed key
            return 1;
        }

        return 0;
    }
    
    char getch() {
        char ch = pressed_key;
        pressed_key = 0; // Reset after reading
        return ch;
    }
    #else
    void enable_raw_mode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
    
    void disable_raw_mode() {
        termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
    }
    
    bool kbhit() {
        timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
    }
    
    char getch() {
        char ch = 0;
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }
    #endif
}