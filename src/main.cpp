#include "./core/core.hpp"
#include <iostream>
#include <memory>

void initialize_scene() {
    auto& entity_manager = CORE::g_entity_manager;
    auto* cube_mesh = &MESHES::static_cube_mesh;
    
    // Create different texture patterns
    auto brick_texture = std::make_shared<TEXTURE::Texture2D>(8, 8, "####@@@@");
    auto wood_texture = std::make_shared<TEXTURE::Texture2D>(6, 6, "|||---");
    auto metal_texture = std::make_shared<TEXTURE::Texture2D>(4, 4, "$$$$####");
    
    auto plain_cube1 = std::make_shared<ENTITY::MeshEntity>(cube_mesh);
    plain_cube1->set_position({0.0f, 0.0f, -4.0f});
    plain_cube1->set_scale({0.5f, 0.5f, 0.5f});
    entity_manager.add_entity(plain_cube1);
    
    // auto plain_cube2 = std::make_shared<ENTITY::MeshEntity>(cube_mesh);
    // plain_cube2->set_position({-2.0f, 0.0f, -4.0f});
    // plain_cube2->set_scale({2.0f, 0.5f, 2.0f});
    // entity_manager.add_entity(plain_cube2);

    // auto plain_cube3 = std::make_shared<ENTITY::MeshEntity>(cube_mesh);
    // plain_cube1->set_position({0.0f, 0.0f, -1.0f});
    // plain_cube1->set_scale({0.5f, 0.5f, 0.5f});
    // entity_manager.add_entity(plain_cube3);
    
    auto plain_cube4 = std::make_shared<ENTITY::MeshEntity>(cube_mesh);
    plain_cube4->set_position({-6.0f, 0.0f, -12.0f});
    plain_cube4->set_scale({2.0f, 0.5f, 2.0f});
    entity_manager.add_entity(plain_cube4);
    
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
        core.set_target_framerate(30); // Lower framerate for ASCII rendering
        
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