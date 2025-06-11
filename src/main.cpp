#include "./core/core.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>

void initialize_scene() {
    auto& entity_manager = CORE::g_entity_manager;
    auto* cube_mesh = &MESHES::static_cube_mesh;
    
    // Create different texture patterns
    auto brick_colors = std::vector<COLOR::RGBColor>{
        {255, 0, 0}, {128, 0, 0}, {200, 100, 100}, {150, 50, 50}
    };
    auto brick_texture = std::make_shared<TEXTURE::ColoredTexture2D>(8, 8, "#", std::vector<COLOR::RGBColor>{
        {255, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}
    });
    auto wood_texture = std::make_shared<TEXTURE::Texture2D>(6, 6, "|||---");
    auto metal_texture = std::make_shared<TEXTURE::Texture2D>(4, 4, "$$$$####");

    // --- Simple maze generation ---
    const int maze_width = 15;
    const int maze_height = 15;
    const float cell_size = 1.0f;
    
    // Create a simple grid pattern with occasional openings
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int y = 0; y < maze_height; ++y) {
        for (int x = 0; x < maze_width; ++x) {
            bool should_place_wall = false;
            
            // Create border walls
            if (x == 0 || x == maze_width - 1 || y == 0 || y == maze_height - 1) {
                should_place_wall = true;
            }
            // Create internal structure - simple grid pattern
            else if (x % 4 == 0 && y % 4 == 0) {
                // Place walls at regular intervals
                should_place_wall = true;
            }
            else if (x % 4 == 0 || y % 4 == 0) {
                // 30% chance to place connecting walls
                should_place_wall = dis(gen) < 0.3;
            }
            // Random scattered walls
            else if (dis(gen) < 0.1) {
                should_place_wall = true;
            }
            
            if (should_place_wall) {
                auto wall = std::make_shared<ENTITY::MeshEntity>(cube_mesh);
                wall->set_position({(float)x * cell_size - 7.0f, 0.0f, (float)y * cell_size - 7.0f});
                wall->set_scale({0.5f, 0.5f, 0.5f});
                entity_manager.add_entity(wall);
            }
        }
    }

    auto plain_cube1 = std::make_shared<ENTITY::TexturedMeshEntity>(cube_mesh, brick_texture);
    plain_cube1->set_position({0.0f, 0.0f, -4.0f});
    plain_cube1->set_scale({0.5f, 0.5f, 0.5f});
    entity_manager.add_entity(plain_cube1);

    std::cout << "Scene initialized with " << entity_manager.get_entities().size() << " entities.\n";
}

void update_scene() {
    // Add any per-frame updates here
    // For example: rotate objects, update animations, etc.
}

void cleanup_scene() {
    std::cout << "Cleaning up scene...\n";
}

int main() {
    try {
        std::cout << "=== 3D ASCII Renderer ===\n";
        std::cout << "Controls:\n";
        std::cout << "  W/S - Move forward/backward\n";
        std::cout << "  A/D - Turn left/right\n";
        std::cout << "  Q   - Quit\n";
        std::cout << "========================\n\n";
        
        // Create core engine instance
        auto core = CORE::Core({100, 50}); // Larger screen for better visibility
        
        // Set up callbacks
        core.on_start = initialize_scene;
        core.on_update = update_scene;
        core.on_late_update = cleanup_scene;
        
        // Configure engine
        core.set_target_framerate(60); // Lower framerate for ASCII rendering
        
        // Initialize and run
        core.initialize();
        core.run_game_loop();
        
        std::cout << "\nThanks for using the 3D ASCII Renderer!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}