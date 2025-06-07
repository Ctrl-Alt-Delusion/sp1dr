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
    // Function declarations only
    #if !defined(_WIN32)
    void enable_raw_mode();
    void disable_raw_mode();
    #endif
    
    bool kbhit();
    char getch();
}