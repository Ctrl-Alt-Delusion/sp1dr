#pragma once
#include "screen.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "../meshes/meshes.hpp"
#include "input_handling.hpp"
#include "../entity/entity.hpp"
#include "../entity/entity_manager.hpp"
#include <functional>
#include <vector>
#include <memory>
#include <chrono>

namespace CORE {

using Vec2Int = MATH::Vec2<int>;
using Vec2Float = MATH::Vec2<float>;
using Vec3Float = MATH::Vec3<float>;
using Vec4Int = MATH::Vec4<int>;

// Configuration constants
namespace Config {
    constexpr float ASPECT_RATIO = 1.8f;
    constexpr float BASE_SCALE = 15.0f;
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 100.0f;
    constexpr float MAX_VIEW_DISTANCE = 27.5f;
    constexpr size_t DEFAULT_FRAME_RATE = 60;
}

// Forward declarations
class Screen;
class FirstPersonCamera;
class ZBuffer;

// Barycentric coordinate calculation for triangle rasterization
struct BarycentricCoords {
    float u, v, w;
    
    constexpr bool is_inside() const noexcept {
        return u >= -1e-6f && v >= -1e-6f && w >= -1e-6f; // Small epsilon for floating point precision
    }
};

// Calculate barycentric coordinates for point p in triangle (a, b, c)
BarycentricCoords calculate_barycentric(const Vec2Int& p, 
                                        const Vec2Int& a, 
                                        const Vec2Int& b, 
                                        const Vec2Int& c) noexcept;

// Enhanced shading functions with lighting support
char get_advanced_shade(float distance, float max_distance, 
                       const Vec3Float& normal, const Vec3Float& light_dir) noexcept;

// Distance-based shading functions
char get_distance_shade(float distance, float max_distance = Config::MAX_VIEW_DISTANCE) noexcept;
char get_enhanced_distance_shade(float distance, float max_distance = Config::MAX_VIEW_DISTANCE) noexcept;

Vec4Int calculate_bounding_box(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2, Screen& screen) noexcept;

// Enhanced triangle rasterization functions
void rasterize_textured_triangle(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2,
                                 float z0, float z1, float z2,
                                 const Vec2Float& uv0, const Vec2Float& uv1, const Vec2Float& uv2,
                                 const ENTITY::TexturedMeshEntity* textured_entity,
                                 Screen& screen,
                                 ZBuffer& z_buffer);

void rasterize_shaded_triangle(const Vec2Int& p0, const Vec2Int& p1, const Vec2Int& p2,
                               float z0, float z1, float z2,
                               const Vec3Float& normal,
                               Screen& screen,
                               class ZBuffer& z_buffer);

// Camera space transformation
Vec3Float world_to_camera_space(const Vec3Float& world_pos, 
                                const FirstPersonCamera& camera) noexcept;

// Projection utilities
Vec2Int project_to_screen(const Vec3Float& camera_pos, 
                          float focal_length,
                          const Vec2Float& screen_size) noexcept;

// Utility function for dot product
constexpr float dot_product(const Vec3Float& a, const Vec3Float& b) noexcept;

// Core engine class
class Core {
public:
    explicit Core(pair_uint screen_size);
    ~Core() = default;
    
    // Non-copyable, movable
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;
    Core(Core&&) = default;
    Core& operator=(Core&&) = default;
    
    void initialize();
    void run_game_loop();
    
    void set_target_framerate(size_t rate) noexcept { _target_frame_rate = rate; }
    
    // Game loop callbacks
    std::function<void()> on_start       = nullptr;
    std::function<void()> on_update      = nullptr;
    std::function<void()> on_late_update = nullptr;

private:
    void update_game_logic(FirstPersonCamera& camera);
    void process_input(FirstPersonCamera& camera);
    bool should_cull_entity(const std::shared_ptr<ENTITY::MeshEntity>& entity, 
                            const FirstPersonCamera& camera) const noexcept;
    
    size_t _target_frame_rate = Config::DEFAULT_FRAME_RATE;
    Screen _screen;
    Renderer _renderer;
    bool _is_running = true;
};

// Enhanced Z-Buffer class for depth testing
class ZBuffer {
private:
    std::vector<std::vector<float>> buffer;
    size_t width, height;

public:
    ZBuffer(size_t w, size_t h) : width(w), height(h) {
        buffer.resize(height, std::vector<float>(width, Config::FAR_PLANE));
    }

    void clear() {
        for (auto& row : buffer) {
            std::fill(row.begin(), row.end(), Config::FAR_PLANE);
        }
    }

    bool test_and_set(size_t x, size_t y, float depth) {
        if (x >= width || y >= height) return false;
        if (depth < buffer[y][x]) {
            buffer[y][x] = depth;
            return true;
        }
        return false;
    }

    float get_depth(size_t x, size_t y) const {
        if (x >= width || y >= height) return Config::FAR_PLANE;
        return buffer[y][x];
    }

    void resize(size_t w, size_t h) {
        width = w;
        height = h;
        buffer.resize(height, std::vector<float>(width, Config::FAR_PLANE));
    }
    
    // Get depth buffer visualization for debugging
    char get_depth_visualization(size_t x, size_t y) const {
        if (x >= width || y >= height) return ' ';
        const float depth = buffer[y][x];
        if (depth >= Config::FAR_PLANE) return ' ';
        return get_distance_shade(depth, Config::MAX_VIEW_DISTANCE);
    }
};

// Global entity manager (consider dependency injection in the future)
extern ENTITY::EntityManager g_entity_manager;

} // namespace CORE