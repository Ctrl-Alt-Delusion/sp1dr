#include "core.hpp"

namespace CORE {
    Core::Core()
    : screen({80, 45}, DEFAULT_SETTINGS), renderer(screen) {
       
    }

    void Core::init() {
        screen.set_pixel({21, 22}, 'X');
        screen.set_pixel({22, 22}, 'X');
        screen.set_pixel({23, 22}, 'X');
        screen.set_pixel({24, 22}, 'X');
        screen.set_pixel({25, 22}, 'X');
    }

    void Core::game_loop() {
        init();
        for (;;) {
            // GAME LOGIC
            if (screen.get_change())
                renderer.render();
                
            screen.set_change(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}