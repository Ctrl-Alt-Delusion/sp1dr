#include "core.hpp"

namespace CORE {

    MESHES::Cube cube {};

    Core::Core(pair_uint screen_size)
    : screen(screen_size, DEFAULT_SETTINGS), renderer(screen) {
       
    }

    void Core::init() {
    }


    // GAME LOGIC FOR CUBE DEMO
    void Core::game_logic(OrbitCamera& camera) {
        const float ASPECT_RATIO = 1.8f;
        const float BASE_SCALE   = 15.0f;
        std::vector<MATH::Vec2<int>> projected;

        // 1) Clear the screen
        for (size_t y = 0; y < screen.get_size().y; ++y)
            for (size_t x = 0; x < screen.get_size().x; ++x)
                screen.set_pixel({x, y}, ' ');

        // 2) For each vertex in cube, rotate by camera's inverse angles, translate, then project
        for (auto v : cube.vertices) {
            // rotate around Y by -yaw, then around X by -pitch
            auto temp = v.rotate_y(-camera.yaw)
                        .rotate_x(-camera.pitch);

            // push it away along Z (camera sits at +Z looking at origin)
            temp.z += camera.dist;

            // perspective projection
            auto p = temp.project(5.0f);

            // aspect correction + center on screen
            p.x = (p.x * BASE_SCALE + 40);
            p.y = (p.y * (BASE_SCALE / ASPECT_RATIO) + 22);

            // clamp
            p.x = std::clamp(p.x, 0.0f, 79.0f);
            p.y = std::clamp(p.y, 0.0f, 44.0f);

            projected.push_back({static_cast<int>(p.x), static_cast<int>(p.y)});
        }

        // 3) Draw edges using those 2D points
        for (const auto& [start, end] : cube.edges) {
            draw_line(projected[start], projected[end], screen, '@');
        }
    }

    void Core::game_loop() {
        OrbitCamera camera;
        #if !defined(_WIN32)
            enable_raw_mode();
        #endif
        
        for (;;) {
            game_logic(camera);
            
            // Handle input
            if (kbhit()) {
                char ch = getch();
                if (ch == 'q')
                    break;
                    
                auto it = std::find(camera.input_char.begin(), camera.input_char.end(), ch);
                if (it != camera.input_char.end()) {
                    int idx = std::distance(camera.input_char.begin(), it);
                    camera.handle_movement((Direction)idx);
                }
            }

            renderer.render();
            
            // Always render every frame
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        #if !defined(_WIN32)
            disable_raw_mode();
        #endif
    }

    void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel) {
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
}