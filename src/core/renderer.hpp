#pragma once

#include "screen.hpp"

// danqzq: sometimes I wonder what if this was all it took to make a game engine..

namespace CORE {

    class Renderer {
    public:
        Renderer(Screen& screen_ref);  
        void render();

    private:
        Screen& screen; 
    };

} // namespace CORE
