#pragma once

#include "screen.hpp"
#include "renderer.hpp"

namespace CORE {

    class Core {
    public:
        Core();
        void game_loop();
    private:
        Screen screen      {};
        Renderer renderer  {};
    };
    
} // CORE