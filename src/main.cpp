#include "./core/core.hpp"

#include <iostream>
#include <memory>

void start() {
    auto cube = &MESHES::static_cube_mesh;
    auto entity_manager = &CORE::entity_manager;
    entity_manager->add_entity(std::make_shared<ENTITY::MeshEntity>(cube));
    
    auto side_cube_entity = std::make_shared<ENTITY::MeshEntity>(cube);
    side_cube_entity->set_position({2.0f, 0.0f, 0.0f});
    entity_manager->add_entity(side_cube_entity);

    side_cube_entity = std::make_shared<ENTITY::MeshEntity>(cube);
    side_cube_entity->set_position({-2.0f, 0.0f, 0.0f});
    entity_manager->add_entity(side_cube_entity);

    side_cube_entity = std::make_shared<ENTITY::MeshEntity>(cube);
    side_cube_entity->set_position({0.0f, -2.0f, 0.0f});
    entity_manager->add_entity(side_cube_entity);
}

void update() {

}

int main() {
    std::cout << "We live bois!" << std::endl;

    auto core = CORE::Core({80, 45});

    core.start = start;
    core.update = update;

    core.init();
    core.game_loop();
    return 0;
}