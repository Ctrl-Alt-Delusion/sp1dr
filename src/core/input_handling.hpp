#pragma once

#include <iostream>
#include <thread>
#include <chrono>

#if defined(_WIN32)
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/select.h>
#endif

namespace CORE {
    // ==== PLATFORM-SPECIFIC: Input Stuff ====
    #if !defined(_WIN32)

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

    #endif

    bool kbhit();
    char getch();
}