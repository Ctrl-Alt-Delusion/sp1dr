#pragma once

#include "screen.hpp"

namespace CORE {

    class Renderer {
    public:
        Renderer(Screen& screen_ref);  
        void render();

    private:
        Screen& screen; 
    };

} // namespace CORE
