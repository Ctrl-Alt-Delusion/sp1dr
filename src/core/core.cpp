#include "core.hpp"

namespace CORE {

    MESHES::Cube cube {};
    MESHES::Cube cube2 {};

    Core::Core(pair_uint screen_size)
    : screen(screen_size, DEFAULT_SETTINGS), renderer(screen) {
       
    }

    void Core::init() {
    }

    char get_depth_shade(float z) {
        z /= 2.0f;
        if (z < 3.0f) return '@';
        else if (z < 3.5f) return '#';
        else if (z < 4.0f) return '*';
        else if (z < 4.5f) return '.';
        return ' ';
    }

    void Core::game_logic(OrbitCamera& camera, ENTITY::EntityManager& manager) {
        const float ASPECT_RATIO = 1.8f;
        const float BASE_SCALE   = 15.0f;

        for (size_t y = 0; y < screen.get_size().y; ++y)
            for (size_t x = 0; x < screen.get_size().x; ++x)
                screen.set_pixel({x, y}, ' ');

        // Clear screen and Z-buffer
        screen.clear_screen();
        std::vector<std::vector<float>> z_buffer(screen.get_size().y, std::vector<float>(screen.get_size().x, std::numeric_limits<float>::infinity()));

        for (const auto& entity : manager.get_entities()) {
                
            std::vector<MATH::Vec2<int>> projected;
            std::vector<MATH::Vec3<float>> camera_space_vertices;
            auto mesh_entity = std::dynamic_pointer_cast<ENTITY::MeshEntity>(entity);
            if (!mesh_entity) continue;

            auto* mesh = mesh_entity->get_mesh();
            auto position = mesh_entity->get_position();
            auto scale    = mesh_entity->get_scale();

            for (auto v : mesh->getVertices()) {
                // Apply scale and position
                v = v.scale(scale) + position;

                // Camera space: rotate based on camera yaw/pitch
                auto cam_v = v.rotate_y(-camera.yaw).rotate_x(-camera.pitch);
                cam_v.z += camera.dist; // Push into view

                camera_space_vertices.push_back(cam_v);

                // Project to 2D
                auto p = cam_v.project(5.0f);
                p.x = (p.x * BASE_SCALE + 40); // Center horizontally
                p.y = (p.y * (BASE_SCALE / ASPECT_RATIO) + 22); // Center vertically

                // Clamp to screen bounds
                p.x = std::clamp(p.x, 0.0f, 79.0f);
                p.y = std::clamp(p.y, 0.0f, 44.0f);

                projected.push_back({static_cast<int>(p.x), static_cast<int>(p.y)});
            }

            // 4) Draw edges between projected vertices
            for (const auto& [start, end] : mesh->getEdges()) {
                const auto& p0 = projected[start];
                const auto& p1 = projected[end];

                float z0 = camera_space_vertices[start].z;
                float z1 = camera_space_vertices[end].z;
                float avg_z = (z0 + z1) * 0.5f;

                draw_line_zbuffered(p0, p1, z0, z1, screen, z_buffer, get_depth_shade(avg_z));
            }
        }
    }


    void Core::game_loop() {
        OrbitCamera camera;
        ENTITY::EntityManager entity_manager;
        
        // Add a test cube
        entity_manager.add_entity(std::make_shared<ENTITY::MeshEntity>(&cube));
        
        auto side_cube_entity = std::make_shared<ENTITY::MeshEntity>(&cube2);
        side_cube_entity->set_position({2.0f, 0.0f, 0.0f});
        entity_manager.add_entity(side_cube_entity);

        side_cube_entity = std::make_shared<ENTITY::MeshEntity>(&cube2);
        side_cube_entity->set_position({-2.0f, 0.0f, 0.0f});
        entity_manager.add_entity(side_cube_entity);

        side_cube_entity = std::make_shared<ENTITY::MeshEntity>(&cube2);
        side_cube_entity->set_position({0.0f, -2.0f, 0.0f});
        entity_manager.add_entity(side_cube_entity);

        #if !defined(_WIN32)
            enable_raw_mode();
        #endif
        
        for (;;) {
            game_logic(camera, entity_manager);
            
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

    void draw_line_zbuffered(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1,
                            float z0, float z1,
                            CORE::Screen& screen,
                            std::vector<std::vector<float>>& z_buffer,
                            char pixel) {
        int dx = std::abs(p1.x - p0.x);
        int dy = std::abs(p1.y - p0.y);
        int sx = (p0.x < p1.x) ? 1 : -1;
        int sy = (p0.y < p1.y) ? 1 : -1;
        int err = dx - dy;

        int x = p0.x;
        int y = p0.y;

        int steps = std::max(dx, dy);
        for (int i = 0; i <= steps; ++i) {
            float t = (steps == 0) ? 0.0f : static_cast<float>(i) / steps;
            float z = z0 + t * (z1 - z0);

            if (x >= 0 && x < (int)screen.get_size().x &&
                y >= 0 && y < (int)screen.get_size().y) {

                if (z < z_buffer[y][x]) {
                    z_buffer[y][x] = z;
                    screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, pixel);
                }
            }

            if (x == p1.x && y == p1.y) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x += sx; }
            if (e2 < dx) { err += dx; y += sy; }
        }
    }

}