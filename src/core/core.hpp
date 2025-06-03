#pragma once

#include "screen.hpp"
#include "renderer.hpp"

#include <thread>
#include <chrono>

namespace CORE {

    class Core {
    public:
        Core();          
        void game_loop();

    private:
        Screen   screen;
        Renderer renderer;
    };

} // namespace CORE
