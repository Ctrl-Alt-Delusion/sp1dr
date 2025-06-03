#include "core.hpp"

namespace CORE {
    Core::Core() {
        screen.set_pixel({21, 22}, 'X');
        screen.set_pixel({22, 22}, 'X');
        screen.set_pixel({23, 22}, 'X');
        screen.set_pixel({24, 22}, 'X');
        screen.set_pixel({25, 22}, 'X');
    }

    void Core::game_loop() {
        for (;;) {

            renderer.render();
        }
    }
}