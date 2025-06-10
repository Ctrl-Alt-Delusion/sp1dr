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

// Configuration constants
namespace Config {
    constexpr float ASPECT_RATIO = 1.8f;
    constexpr float BASE_SCALE = 15.0f;
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 100.0f;
    constexpr float MAX_VIEW_DISTANCE = 25.0f;
    constexpr size_t DEFAULT_FRAMERATE = 60;
}

// Forward declarations
class Screen;
class FirstPersonCamera;

// Barycentric coordinate calculation for triangle rasterization
struct BarycentricCoords {
    float u, v, w;
    
    constexpr bool is_inside() const noexcept {
        return u >= -1e-6f && v >= -1e-6f && w >= -1e-6f; // Small epsilon for floating point precision
    }
};

// Calculate barycentric coordinates for point p in triangle (a, b, c)
BarycentricCoords calculate_barycentric(const MATH::Vec2<int>& p, 
                                       const MATH::Vec2<int>& a, 
                                       const MATH::Vec2<int>& b, 
                                       const MATH::Vec2<int>& c) noexcept;

// Distance-based shading functions
char get_distance_shade(float distance, float max_distance = Config::MAX_VIEW_DISTANCE) noexcept;
char get_enhanced_distance_shade(float distance, float max_distance = Config::MAX_VIEW_DISTANCE) noexcept;

// Triangle rasterization functions
void rasterize_textured_triangle(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, const MATH::Vec2<int>& p2,
                                float z0, float z1, float z2,
                                const ENTITY::TexturedMeshEntity* textured_entity,
                                Screen& screen,
                                std::vector<std::vector<float>>& z_buffer);

void rasterize_shaded_triangle(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, const MATH::Vec2<int>& p2,
                              float z0, float z1, float z2,
                              Screen& screen,
                              std::vector<std::vector<float>>& z_buffer,
                              char fill_char = '#');

// Camera space transformation
MATH::Vec3<float> world_to_camera_space(const MATH::Vec3<float>& world_pos, 
                                       const FirstPersonCamera& camera) noexcept;

// Projection utilities
MATH::Vec2<int> project_to_screen(const MATH::Vec3<float>& camera_pos, 
                                 float focal_length,
                                 const MATH::Vec2<float>& screen_size) noexcept;

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
    
    void set_target_framerate(size_t rate) noexcept { target_framerate_ = rate; }
    
    // Game loop callbacks
    std::function<void()> on_start = nullptr;
    std::function<void()> on_update = nullptr;
    std::function<void()> on_late_update = nullptr;

private:
    void update_game_logic(FirstPersonCamera& camera);
    void process_input(FirstPersonCamera& camera);
    bool should_cull_entity(const std::shared_ptr<ENTITY::MeshEntity>& entity, 
                           const FirstPersonCamera& camera) const noexcept;
    
    size_t target_framerate_ = Config::DEFAULT_FRAMERATE;
    Screen screen_;
    Renderer renderer_;
    bool is_running_ = true;
};

// Global entity manager (consider dependency injection in the future)
extern ENTITY::EntityManager g_entity_manager;

} // namespace CORE