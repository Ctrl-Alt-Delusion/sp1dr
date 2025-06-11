#include "core.hpp"
#include <algorithm>
#include <cmath>
#include <thread>
#include <iostream>

namespace CORE {

// Global entity manager
ENTITY::EntityManager g_entity_manager{};

// Barycentric coordinate calculation
BarycentricCoords calculate_barycentric(const Vec2Int& p, 
                                        const Vec2Int& a, 
                                        const Vec2Int& b, 
                                        const Vec2Int& c) noexcept {
    const Vec2Int v0 = {c.x - a.x, c.y - a.y};
    const Vec2Int v1 = {b.x - a.x, b.y - a.y};
    const Vec2Int v2 = {p.x - a.x, p.y - a.y};

    const int dot00 = v0.x * v0.x + v0.y * v0.y;
    const int dot01 = v0.x * v1.x + v0.y * v1.y;
    const int dot02 = v0.x * v2.x + v0.y * v2.y;
    const int dot11 = v1.x * v1.x + v1.y * v1.y;
    const int dot12 = v1.x * v2.x + v1.y * v2.y;

    const float denom = static_cast<float>(dot00 * dot11 - dot01 * dot01);
    if (std::abs(denom) < 1e-10f) {
        return {0.0f, 0.0f, 0.0f}; // Degenerate triangle
    }

    const float inv_denom = 1.0f / denom;
    const float u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    const float v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
    const float w = 1.0f - u - v;

    return {u, v, w};
}

// Distance-based shading
char get_distance_shade(float distance, float max_distance) noexcept {
    if (distance > max_distance) return ' ';
    
    const float normalized = std::clamp(distance / max_distance, 0.0f, 1.0f);
    
    // Optimized character array - fewer lookups
    constexpr char shade_chars[] = {'@', '#', '8', '&', 'o', ':', '*', '.', '~', '-', '^', '\'', ' '};
    constexpr int num_chars = sizeof(shade_chars) - 1;
    
    const int index = static_cast<int>(normalized * (num_chars - 1));
    return shade_chars[std::clamp(index, 0, num_chars - 1)];
}

char get_enhanced_distance_shade(float distance, float max_distance) noexcept {
    if (distance > max_distance) return ' ';
    
    const float normalized = std::clamp(distance / max_distance, 0.0f, 1.0f);
    
    // Extended character set for smoother gradients
    constexpr char shade_chars[] = {
        '@', '#', '8', '&', '%', 'B', 'M', 'N', 'W', 'Q', 'R', 'O', 'o', 'a', 'h',
        'k', 'b', 'd', 'p', 'q', 'w', 'm', 'Z', 'X', 'Y', 'U', 'J', 'C', 'L',
        'z', 'c', 'v', 'u', 'n', 'x', 'r', 'j', 'f', 't', '/', '\\', '|', '(',
        '1', '{', '}', '[', ']', '?', '-', '_', '+', '~', '<', '>', 'i', '!', 'l',
        'I', ';', ':', ',', '"', '^', '`', '\'', '.', ' '
    };
    constexpr int num_chars = sizeof(shade_chars) - 1;
    
    const int index = static_cast<int>(normalized * (num_chars - 1));
    return shade_chars[std::clamp(index, 0, num_chars - 1)];
}

// Utility function for dot product
constexpr float dot_product(const Vec3Float& a, const Vec3Float& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Camera space transformation
Vec3Float world_to_camera_space(const Vec3Float& world_pos, 
                                const FirstPersonCamera& camera) noexcept {
    // Precompute trigonometric values
    const float cos_pitch = std::cos(camera.pitch);
    const float sin_pitch = std::sin(camera.pitch);
    const float cos_yaw = std::cos(camera.yaw);
    const float sin_yaw = std::sin(camera.yaw);

    // Camera basis vectors
    const Vec3Float forward = {
        cos_pitch * sin_yaw,
        sin_pitch,
        cos_pitch * cos_yaw
    };

    const Vec3Float world_up = {0.0f, 1.0f, 0.0f};
    const Vec3Float right = world_up.cross(forward).normalize();
    const Vec3Float up = forward.cross(right);

    // Transform to camera space
    const Vec3Float relative = world_pos - camera.position;
    
    return {
        dot_product(relative, right),
        dot_product(relative, up),
        dot_product(relative, forward)
    };
}

// Screen projection
Vec2Int project_to_screen(const Vec3Float& camera_pos, 
                          float focal_length,
                          const Vec2Float& screen_size) noexcept {
    if (camera_pos.z >= -Config::NEAR_PLANE) {
        return {-1, -1}; // Behind camera
    }

    const auto projected = camera_pos.project(focal_length);
    const float x = projected.x * Config::BASE_SCALE + screen_size.x * 0.5f;
    const float y = projected.y * (Config::BASE_SCALE / Config::ASPECT_RATIO) + screen_size.y * 0.5f;

    return {
        static_cast<int>(std::clamp(x, 0.0f, screen_size.x - 1.0f)),
        static_cast<int>(std::clamp(y, 0.0f, screen_size.y - 1.0f))
    };
}

Vec4Int calculate_bounding_box(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2, Screen& screen) noexcept {
    const auto screen_size = screen.get_size();
    const int min_x = std::max(0, std::min({p0.x, p1.x, p2.x}));
    const int max_x = std::min(static_cast<int>(screen_size.x) - 1, std::max({p0.x, p1.x, p2.x}));
    const int min_y = std::max(0, std::min({p0.y, p1.y, p2.y}));
    const int max_y = std::min(static_cast<int>(screen_size.y) - 1, std::max({p0.y, p1.y, p2.y}));

    return {min_x, min_y, max_x, max_y};
}

void rasterize_textured_triangle(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2,
                                        float z0, float z1, float z2,
                                        const ENTITY::TexturedMeshEntity* textured_entity,
                                        Screen& screen,
                                        ZBuffer& z_buffer) {
    const auto [min_x, min_y, max_x, max_y] = calculate_bounding_box(p0, p1, p2, screen);

    // Precompute triangle area for barycentric calculation optimization
    const float triangle_area = static_cast<float>(
        (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y)
    );
    
    if (std::abs(triangle_area) < 1e-6f) {
        return; // Degenerate triangle
    }

    const float inv_area = 1.0f / triangle_area;

    // Rasterize pixels within bounding box
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            const Vec2Int pixel = {x, y};
            
            // Calculate barycentric coordinates
            const BarycentricCoords bary = calculate_barycentric(pixel, p0, p1, p2);
            
            if (!bary.is_inside()) {
                continue;
            }

            // Interpolate depth using barycentric coordinates
            const float depth = bary.u * z0 + bary.v * z1 + bary.w * z2;
            
            // Early depth test
            if (depth > Config::MAX_VIEW_DISTANCE || depth < Config::NEAR_PLANE) {
                continue;
            }

            // Z-buffer test and update
            if (z_buffer.test_and_set(static_cast<size_t>(x), static_cast<size_t>(y), depth)) {
                // UV mapping using barycentric coordinates
                // Note: You might need to adjust this based on your UV coordinate system
                const float u = bary.v; // These mappings might need adjustment
                const float v = bary.w; // based on your specific UV layout

                const char tex_char = textured_entity->get_texture_char(u, v);
                
                // Only render non-transparent pixels
                if (tex_char != ' ' && tex_char != '\0') {
                    screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, tex_char);
                }
            }
        }
    }
}

// 3. Enhanced shaded triangle rasterization for consistency
void rasterize_shaded_triangle(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2,
                                      float z0, float z1, float z2,
                                      Screen& screen,
                                      ZBuffer& z_buffer) {
    const auto [min_x, min_y, max_x, max_y] = calculate_bounding_box(p0, p1, p2, screen);

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            const Vec2Int pixel = {x, y};
            const BarycentricCoords bary = calculate_barycentric(pixel, p0, p1, p2);
            
            if (!bary.is_inside()) {
                continue;
            }

            const float depth = bary.u * z0 + bary.v * z1 + bary.w * z2;
            
            if (depth > Config::MAX_VIEW_DISTANCE || depth < Config::NEAR_PLANE) {
                continue;
            }

            if (z_buffer.test_and_set(static_cast<size_t>(x), static_cast<size_t>(y), depth)) {
                const char shade_char = get_distance_shade(depth);
                if (shade_char != ' ') {
                    screen.set_pixel({static_cast<size_t>(x), static_cast<size_t>(y)}, shade_char);
                }
            }
        }
    }
}

// Core class implementation
Core::Core(pair_uint screen_size) 
    : _screen(screen_size, DEFAULT_SETTINGS), _renderer(_screen) {
}

void Core::initialize() {
    if (on_start) {
        on_start();
    }
}

bool Core::should_cull_entity(const std::shared_ptr<ENTITY::MeshEntity>& entity, 
                              const FirstPersonCamera& camera) const noexcept {
    const auto position = entity->get_position();
    const auto scale = entity->get_scale();
    
    const Vec3Float entity_to_camera = position - camera.position;
    const float entity_distance = entity_to_camera.magnitude();
    
    // Frustum culling based on distance
    const float max_scale = std::max({scale.x, scale.y, scale.z});
    return entity_distance > Config::MAX_VIEW_DISTANCE + max_scale;
}

void Core::update_game_logic(FirstPersonCamera& camera) {
    const auto screen_size_uint = _screen.get_size();
    const Vec2Float screen_size = {
        static_cast<float>(screen_size_uint.x), 
        static_cast<float>(screen_size_uint.y)
    };

    // Clear screen buffer
    for (size_t y = 0; y < screen_size_uint.y; ++y) {
        for (size_t x = 0; x < screen_size_uint.x; ++x) {
            _screen.set_pixel({x, y}, ' ');
        }
    }

    // Initialize Z-buffer with improved class
    ZBuffer z_buffer(screen_size_uint.x, screen_size_uint.y);

    // Collect and sort entities by distance for better rendering order
    struct EntityWithDistance {
        std::shared_ptr<ENTITY::MeshEntity> entity;
        float distance;
    };

    std::vector<EntityWithDistance> sorted_entities;
    
    for (const auto& entity : g_entity_manager.get_entities()) {
        auto mesh_entity = std::dynamic_pointer_cast<ENTITY::MeshEntity>(entity);
        if (!mesh_entity || should_cull_entity(mesh_entity, camera)) {
            continue;
        }

        const Vec3Float entity_to_camera = mesh_entity->get_position() - camera.position;
        const float distance = entity_to_camera.magnitude();
        
        sorted_entities.push_back({mesh_entity, distance});
    }

    // Sort entities back-to-front for better transparency handling
    std::sort(sorted_entities.begin(), sorted_entities.end(),
              [](const EntityWithDistance& a, const EntityWithDistance& b) {
                  return a.distance > b.distance;
              });

    // Render all entities
    for (const auto& entity_data : sorted_entities) {
        const auto& mesh_entity = entity_data.entity;

        // Transform and project vertices
        std::vector<Vec2Int> projected_vertices;
        std::vector<Vec3Float> camera_vertices;
        std::vector<float> depths;

        const auto* mesh     = mesh_entity->get_mesh();
        const auto  position = mesh_entity->get_position();
        const auto  scale    = mesh_entity->get_scale();

        for (const auto& vertex : mesh->getVertices()) {
            const Vec3Float world_pos  = vertex.scale(scale) + position;
            const Vec3Float cam_vertex = world_to_camera_space(world_pos, camera);
            
            camera_vertices.push_back(cam_vertex);
            depths.push_back(cam_vertex.magnitude());
            
            const Vec2Int projected = project_to_screen(cam_vertex, camera.focal_length, screen_size);
            projected_vertices.push_back(projected);
        }

        const auto textured_entity = std::dynamic_pointer_cast<ENTITY::TexturedMeshEntity>(mesh_entity);
        
        // Render faces
        for (const auto& face : mesh->getFaces()) {
            // Skip if any vertex is behind camera
            if (projected_vertices[face.x].x < 0 || 
                projected_vertices[face.y].x < 0 || 
                projected_vertices[face.z].x < 0) {
                continue;
            }

            const auto& p0 = projected_vertices[face.x];
            const auto& p1 = projected_vertices[face.y];
            const auto& p2 = projected_vertices[face.z];

            const float z0 = depths[face.x];
            const float z1 = depths[face.y];
            const float z2 = depths[face.z];

            // Distance culling
            const float avg_depth = (z0 + z1 + z2) / 3.0f;
            if (avg_depth > Config::MAX_VIEW_DISTANCE) {
                continue;
            }

            // Backface culling
            const Vec3Float& v0_cam = camera_vertices[face.x];
            const Vec3Float& v1_cam = camera_vertices[face.y];
            const Vec3Float& v2_cam = camera_vertices[face.z];
            
            const Vec3Float edge1 = v1_cam - v0_cam;
            const Vec3Float edge2 = v2_cam - v0_cam;
            const Vec3Float normal = edge1.cross(edge2);
            
            const Vec3Float face_center = (v0_cam + v1_cam + v2_cam) * (1.0f/3.0f);
            const Vec3Float view_dir = -face_center.normalize();
            
            if (dot_product(normal, view_dir) <= 0) {
                continue; // Skip backfacing triangles
            }

            // Render triangle with improved z-buffering
            if (textured_entity) {
                rasterize_textured_triangle(p0, p1, p2, z0, z1, z2, 
                                                   textured_entity.get(), _screen, z_buffer);
            } else {
                rasterize_shaded_triangle(p0, p1, p2, z0, z1, z2, 
                                                 _screen, z_buffer);
            }
        }
    }
}

void Core::process_input(FirstPersonCamera& camera) {
    if (!is_any_key_pressed()) {
        return;
    }

    if (is_key_pressed(KeyCode::KEY_ESCAPE)) {
        _is_running = false;
        return;
    }
    
    const char input = get_pressed_key();
    const auto& input_chars = camera.input_char;
    const auto it = std::find(input_chars.begin(), input_chars.end(), input);
    if (it != input_chars.end()) {
        const int direction_index = std::distance(input_chars.begin(), it);
        camera.handle_movement(static_cast<Direction>(direction_index));
    }
}

void Core::run_game_loop() {
    FirstPersonCamera camera;

    #if !defined(_WIN32)
        enable_raw_mode();
    #endif
    
    const auto frame_duration = std::chrono::milliseconds(1000 / _target_frame_rate);
    
    while (_is_running) {
        const auto frame_start = std::chrono::steady_clock::now();
        
        // Update game logic
        if (on_update) {
            on_update();
        }
        
        update_game_logic(camera);
        process_input(camera);
        
        // Render
        _screen.clear_screen();
        _renderer.render();
        
        // Late update
        if (on_late_update) {
            on_late_update();
        }
        
        // Frame rate limiting
        const auto frame_end = std::chrono::steady_clock::now();
        const auto elapsed = frame_end - frame_start;
        if (elapsed < frame_duration) {
            std::this_thread::sleep_for(frame_duration - elapsed);
        }
    }
    
    #if !defined(_WIN32)
        disable_raw_mode();
    #endif
}

} // namespace CORE