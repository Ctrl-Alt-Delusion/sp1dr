#include "input_handling.hpp"

#ifdef _WIN32
#include <windows.h>
#include <cctype>  // for tolower
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstdio>
#endif

namespace CORE {

#ifdef _WIN32

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

#else // Linux / macOS

static struct termios original_termios;
static bool raw_mode_enabled = false;

static char input_buffered_char = 0;
static bool input_char_ready = false;

void enable_raw_mode() {
    if (raw_mode_enabled) return;

    tcgetattr(STDIN_FILENO, &original_termios);

    termios raw = original_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    raw_mode_enabled = true;
}

void disable_raw_mode() {
    if (!raw_mode_enabled) return;

    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    raw_mode_enabled = false;
}

// This just checks if there's *any* input, but doesn't consume it
bool is_any_key_pressed() {
    timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0;
}

// Reads and caches one character into the buffer
void poll_input() {
    if (input_char_ready) return;

    char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n == 1) {
        input_buffered_char = ch;
        input_char_ready = true;
    }
}

char get_pressed_key() {
    if (input_char_ready) {
        input_char_ready = false;
        return input_buffered_char;
    }

    char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    return (n == 1) ? ch : 0;
}

bool is_key_pressed(KeyCode key) {
    poll_input(); // Ensure buffer is populated
    if (!input_char_ready) return false;

    return tolower(input_buffered_char) == tolower(key);
}

#endif // Platform

} // namespace CORE
