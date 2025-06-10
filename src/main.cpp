// Updated main.cpp with textured cubes
#include "./core/core.hpp"
#include <iostream>
#include <memory>

void start() {
    auto cube = &MESHES::static_cube_mesh;
    auto entity_manager = &CORE::entity_manager;

    // Create different textures
    auto brick_texture = std::make_shared<TEXTURE::Texture2D>(8, 8, "####");
    auto wood_texture = std::make_shared<TEXTURE::Texture2D>(6, 6, "@@@@@");
    auto metal_texture = std::make_shared<TEXTURE::Texture2D>(4, 4, "$$$$$$$");

    // Create textured cubes
    auto textured_cube1 = std::make_shared<ENTITY::MeshEntity>(cube);
    textured_cube1->set_position({3.0f, 0.0f, -6.0f});
    entity_manager->add_entity(textured_cube1);

    auto textured_cube2 = std::make_shared<ENTITY::MeshEntity>(cube);
    textured_cube2->set_position({0.0f, 0.0f, -6.0f});
    entity_manager->add_entity(textured_cube2);

    textured_cube1 = std::make_shared<ENTITY::MeshEntity>(cube);
    textured_cube1->set_position({-3.0f, 0.0f, -6.0f});
    entity_manager->add_entity(textured_cube1);

    // // Add a non-textured cube for comparison
    // auto plain_cube = std::make_shared<ENTITY::MeshEntity>(cube);
    // plain_cube->set_position({0.0f, 2.0f, 0.0f});
    // entity_manager->add_entity(plain_cube);
}

void update() {
}

int main() {
    std::cout << "Textured 3D Renderer Started!" << std::endl;
    
    auto core = CORE::Core({80, 45}); // Larger screen for better texture visibility
    core.start = start;
    core.update = update;
    core.init();
    core.game_loop();
    
    return 0;
}