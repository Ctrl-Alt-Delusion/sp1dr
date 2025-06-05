#pragma once

#include "screen.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "../demo/cube_demo.hpp"
#include "input_handling.hpp"

#include <algorithm>
#include <thread>
#include <chrono>

namespace CORE {

    extern CUBE_DEMO::Cube cube;
    class Core {
    public:
        Core();          
        void game_loop();
        
        void game_logic();
        void game_logic(OrbitCamera& camera);

        void init();

    private:
        Screen   screen;
        Renderer renderer;
    };

    void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel = '#');

} // namespace CORE
