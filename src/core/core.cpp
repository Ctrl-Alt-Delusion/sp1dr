#include "core.hpp"

namespace CORE {
    char bg_char = ' ';
    ENTITY::EntityManager entity_manager {};

    // Calculate barycentric coordinates for point p in triangle (a, b, c)
    BarycentricCoords barycentric(const MATH::Vec2<int>& p, 
                                 const MATH::Vec2<int>& a, 
                                 const MATH::Vec2<int>& b, 
                                 const MATH::Vec2<int>& c) {
        MATH::Vec2<int> v0 = {c.x - a.x, c.y - a.y};
        MATH::Vec2<int> v1 = {b.x - a.x, b.y - a.y};
        MATH::Vec2<int> v2 = {p.x - a.x, p.y - a.y};

        int dot00 = v0.x * v0.x + v0.y * v0.y;
        int dot01 = v0.x * v1.x + v0.y * v1.y;
        int dot02 = v0.x * v2.x + v0.y * v2.y;
        int dot11 = v1.x * v1.x + v1.y * v1.y;
        int dot12 = v1.x * v2.x + v1.y * v2.y;

        float inv_denom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
        float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
        float w = 1.0f - u - v;

        return {u, v, w};
    }

    // Rasterize a textured triangle
    void draw_textured_triangle(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, const MATH::Vec2<int>& p2,
                               float z0, float z1, float z2,
                               const ENTITY::TexturedMeshEntity* textured_entity,
                               CORE::Screen& screen,
                               std::vector<std::vector<float>>& z_buffer) {
        
        // Find bounding box
        int min_x = std::max(0, std::min({p0.x, p1.x, p2.x}));
        int max_x = std::min((int)screen.get_size().x - 1, std::max({p0.x, p1.x, p2.x}));
        int min_y = std::max(0, std::min({p0.y, p1.y, p2.y}));
        int max_y = std::min((int)screen.get_size().y - 1, std::max({p0.y, p1.y, p2.y}));

        // Rasterize each pixel in bounding box
        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                MATH::Vec2<int> p = {x, y};
                BarycentricCoords bary = barycentric(p, p0, p1, p2);
                
                if (bary.is_inside()) {
                    // Interpolate depth
                    float z = bary.u * z0 + bary.v * z1 + bary.w * z2;
                    
                    if (z < z_buffer[y][x]) {
                        z_buffer[y][x] = z;
                        
                        // Simple UV mapping (you can make this more sophisticated)
                        float u = bary.v; // Map barycentric coords to UV
                        float v = bary.w;
                        
                        char tex_char = textured_entity->get_texture_char(u, v);
                        screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, tex_char);
                    }
                }
            }
        }
    }

    // Simple triangle rasterization without texture (fallback)
    void draw_triangle(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, const MATH::Vec2<int>& p2,
                      float z0, float z1, float z2,
                      CORE::Screen& screen,
                      std::vector<std::vector<float>>& z_buffer,
                      char fill_char = '#') {
        
        // Find bounding box
        int min_x = std::max(0, std::min({p0.x, p1.x, p2.x}));
        int max_x = std::min((int)screen.get_size().x - 1, std::max({p0.x, p1.x, p2.x}));
        int min_y = std::max(0, std::min({p0.y, p1.y, p2.y}));
        int max_y = std::min((int)screen.get_size().y - 1, std::max({p0.y, p1.y, p2.y}));

        // Rasterize each pixel in bounding box
        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                MATH::Vec2<int> p = {x, y};
                BarycentricCoords bary = barycentric(p, p0, p1, p2);
                
                if (bary.is_inside()) {
                    // Interpolate depth
                    float z = bary.u * z0 + bary.v * z1 + bary.w * z2;
                    
                    if (z < z_buffer[y][x]) {
                        z_buffer[y][x] = z;
                        screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, fill_char);
                    }
                }
            }
        }

    }

    Core::Core(pair_uint screen_size)
    : screen(screen_size, DEFAULT_SETTINGS), renderer(screen) {
       
    }

    void Core::init() {
        if (start)
            start();
    }

    void Core::game_logic(OrbitCamera& camera) {
        const float ASPECT_RATIO = 1.8f;
        const float BASE_SCALE   = 15.0f;

        auto screen_size_uint = screen.get_screen_size();
        MATH::Vec2<float> screen_size = {static_cast<float>(screen_size_uint.x), static_cast<float>(screen_size_uint.y)};

        // Clear screen
        for (size_t y = 0; y < screen.get_size().y; ++y)
            for (size_t x = 0; x < screen.get_size().x; ++x)
                screen.set_pixel({x, y}, bg_char);

        screen.clear_screen();
        std::vector<std::vector<float>> z_buffer(screen.get_size().y, std::vector<float>(screen.get_size().x, std::numeric_limits<float>::infinity()));

        for (const auto& entity : entity_manager.get_entities()) {
            auto mesh_entity = std::dynamic_pointer_cast<ENTITY::MeshEntity>(entity);
            if (!mesh_entity) continue;

            std::vector<MATH::Vec2<int>> projected;
            std::vector<MATH::Vec3<float>> camera_space_vertices;

            auto* mesh = mesh_entity->get_mesh();
            auto position = mesh_entity->get_position();
            auto scale = mesh_entity->get_scale();

            // Transform and project vertices
            for (auto v : mesh->getVertices()) {
                // Apply scale and position
                v = v.scale(scale) + position;

                // Camera space: rotate based on camera yaw/pitch
                auto cam_v = v.rotate_y(-camera.yaw).rotate_x(-camera.pitch);
                cam_v.z += camera.dist; // Push into view

                camera_space_vertices.push_back(cam_v);

                // Project to 2D
                auto p = cam_v.project(camera.focal_length);
                p.x = (p.x * BASE_SCALE + screen_size.x * 0.5f);
                p.y = (p.y * (BASE_SCALE / ASPECT_RATIO) + screen_size.y * 0.5f);

                // Clamp to screen bounds
                p.x = std::clamp(p.x, 0.0f, screen_size.x - 1.0f);
                p.y = std::clamp(p.y, 0.0f, screen_size.y - 1.0f);

                projected.push_back({static_cast<int>(p.x), static_cast<int>(p.y)});
            }

            // Check if this is a textured entity
            auto textured_entity = std::dynamic_pointer_cast<ENTITY::TexturedMeshEntity>(entity);
            
            // Draw faces (triangles)
            for (const auto& face : mesh->getFaces()) {
                const auto& p0 = projected[face.x];
                const auto& p1 = projected[face.y];
                const auto& p2 = projected[face.z];

                float z0 = camera_space_vertices[face.x].z;
                float z1 = camera_space_vertices[face.y].z;
                float z2 = camera_space_vertices[face.z].z;

                // Simple backface culling (optional)
                MATH::Vec3<float> v0_cam = camera_space_vertices[face.x];
                MATH::Vec3<float> v1_cam = camera_space_vertices[face.y];
                MATH::Vec3<float> v2_cam = camera_space_vertices[face.z];
                
                MATH::Vec3<float> edge1 = v1_cam - v0_cam;
                MATH::Vec3<float> edge2 = v2_cam - v0_cam;
                MATH::Vec3<float> normal = edge1.cross(edge2);
                
                // If normal points away from camera, skip this face
                if (normal.z > 0) continue;

                if (textured_entity) {
                    draw_textured_triangle(p0, p1, p2, z0, z1, z2, textured_entity.get(), screen, z_buffer);
                } else {
                    // Use depth-based shading for non-textured meshes
                    float avg_z = (z0 + z1 + z2) / 3.0f;
                    draw_triangle(p0, p1, p2, z0, z1, z2, screen, z_buffer, get_depth_shade(avg_z));
                }
            }
            for (const auto& [start, end] : mesh->getEdges()) {
                const auto& p0 = projected[start];
                const auto& p1 = projected[end];
                
                float z0 = camera_space_vertices[start].z;
                float z1 = camera_space_vertices[end].z;
                
                draw_line_zbuffered(p0, p1, z0, z1, screen, z_buffer, '|');
            }
        }
    }

    void Core::game_loop() {
        OrbitCamera camera;

        #if !defined(_WIN32)
            enable_raw_mode();
        #endif
        
        for (;;) {
            if (update)
                update();
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
            
            if (late_update)
                late_update();
                
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / target_frame_rate));
        }
        
        #if !defined(_WIN32)
            disable_raw_mode();
        #endif
    }

    char get_depth_shade(float z) {
        z /= 2.0f;
        if (z < 3.0f) return '@';
        else if (z < 3.5f) return '#';
        else if (z < 4.0f) return '*';
        else if (z < 4.5f) return '.';
        return ' ';
    }

    // void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel) {
    //     int dx = std::abs(p1.x - p0.x);
    //     int dy = std::abs(p1.y - p0.y);
    //     int sx = (p0.x < p1.x) ? 1 : -1;
    //     int sy = (p0.y < p1.y) ? 1 : -1;
    //     int err = dx - dy;

    //     int x = p0.x;
    //     int y = p0.y;

    //     while (true) {
    //         screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, pixel);
    //         if (x == p1.x && y == p1.y) break;
    //         int e2 = 2 * err;
    //         if (e2 > -dy) { err -= dy; x += sx; }
    //         if (e2 < dx) { err += dx; y += sy; }
    //     }
    // }

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