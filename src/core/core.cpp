#include "core.hpp"

namespace CORE {

    CUBE_DEMO::Cube cube {};

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


    // GAME LOGIC FOR CUBE DEMO
    void Core::game_logic(float angle) {
        std::vector<MATH::Vec2<int>> projected;
        
        // Clear the screen buffer before drawing the new frame
        for (size_t y = 0; y < screen.get_size().y; ++y) {
            for (size_t x = 0; x < screen.get_size().x; ++x) {
                screen.set_pixel({x, y}, ' ');
            }
        }
        
        // ASPECT RATIO CORRECTION CONSTANTS
        // Most console fonts have characters that are approximately 1.8-2.0 times taller than wide
        // We compensate by scaling Y coordinates less than X coordinates
        const float ASPECT_RATIO = 1.8f;  // Typical character height/width ratio
        const float BASE_SCALE = 12.0f;   // Base scaling factor for visibility
        
        // Transform each vertex of the cube in 3D space
        for (auto v : cube.vertices) {
            // Apply rotations to create the spinning animation
            v = v.rotate_y(angle).rotate_x(angle * 0.5f);
            
            // Move cube away from camera to avoid z-division problems
            v.z += 4.0f; // Sweet spot that gives good perspective without extreme distortion
            
            // Project the 3D point to 2D screen coordinates
            auto p = v.project(5.0f);
            
            // ASPECT RATIO COMPENSATION: Apply different scaling to X and Y
            // X-axis: Use full scaling to take advantage of horizontal screen space
            p.x = (p.x * BASE_SCALE + 40);  // Center horizontally (screen_width/2)
            
            // Y-axis: Reduce scaling by the aspect ratio to compensate for tall characters
            // This makes vertical distances appear proportionally correct
            p.y = (p.y * (BASE_SCALE / ASPECT_RATIO) + 22);  // Center vertically, aspect-corrected
            
            // Alternative mathematical approach - you could also think of it this way:
            // p.y = (p.y * BASE_SCALE * 0.56 + 22);  // 0.56 ≈ 1/1.8, the inverse ratio
            
            // Clamp coordinates to screen bounds for safety
            p.x = std::max(0.0f, std::min(79.0f, p.x));
            p.y = std::max(0.0f, std::min(44.0f, p.y));
            
            projected.push_back({static_cast<int>(p.x), static_cast<int>(p.y)});
        }
        
        // Draw all edges of the cube using the aspect-corrected coordinates
        for (const auto& [start, end] : cube.edges) {
            draw_line(projected[start], projected[end], screen, '@');
        }
    }

    void Core::game_loop() {
        init();
        float angle = 0.0f;
        for (;;) {
            game_logic(angle);

            if (screen.get_change())
                renderer.render();
                
            screen.set_change(0);
            angle += 0.03f;
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}