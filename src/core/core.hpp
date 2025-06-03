#pragma once

#include "screen.hpp"
#include "renderer.hpp"
#include "../demo/cube_demo.hpp"

#include <thread>
#include <chrono>

namespace CORE {

    extern CUBE_DEMO::Cube cube;
    class Core {
    public:
        Core();          
        void game_loop();
        
        void game_logic();
        void game_logic(float angle);

        void init();

    private:
        Screen   screen;
        Renderer renderer;
    };

    void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel = '#') {
        int dx = std::abs(p1.x - p0.x);
        int dy = std::abs(p1.y - p0.y);
        int sx = (p0.x < p1.x) ? 1 : -1;
        int sy = (p0.y < p1.y) ? 1 : -1;
        int err = dx - dy;

        int x = p0.x;
        int y = p0.y;

        while (true) {
            screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, pixel);
            if (x == p1.x && y == p1.y) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x += sx; }
            if (e2 < dx) { err += dx; y += sy; }
        }
    }

} // namespace CORE
