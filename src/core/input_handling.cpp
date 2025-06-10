#include "input_handling.hpp"

namespace CORE {
    // ==== PLATFORM-SPECIFIC: Input Stuff ====
    #if defined(_WIN32)
    char currently_pressed_key = 0;
    bool is_any_key_pressed() {
        char ch = 0;
        for (unsigned char i = 32; i <= 127; i++) {
            if (GetAsyncKeyState(i) & 0x8000) {
                ch = tolower(i);
                break;
            }
        }
        
        if (ch != 0) {
            currently_pressed_key = ch; // Store the pressed key
            return 1;
        }

        return 0;
    }

    bool is_key_pressed(KeyCode key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }
    
    char get_pressed_key() {
        char ch = currently_pressed_key;
        currently_pressed_key = 0; // Reset after reading
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
    
    bool is_any_key_pressed() {
        timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
    }
    
    char get_pressed_key() {
        char ch = 0;
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }

    bool is_key_pressed(KeyCode key) {
        termios old_term, new_term;
        tcgetattr(STDIN_FILENO, &old_term);
        new_term = old_term;
        new_term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
        
        char ch = get_pressed_key();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        
        return ch == key;
    }
    #endif
}