#include "./core/core.hpp"
#include <memory>
#include <random>

void initialize_scene() {
    auto& em    = CORE::g_entity_manager;
    auto* mesh  = &MESHES::static_cube_mesh;

    auto brick  = std::make_shared<TEXTURE::Texture2D>(8, 8, "#-#-#-#");
    auto wood   = std::make_shared<TEXTURE::Texture2D>(6, 6, "~||~||~");
    auto metal  = std::make_shared<TEXTURE::Texture2D>(4, 4, "@#@#@#@#");

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::uniform_int_distribution<> tex(0, 2);

    const int width  = 17;
    const int height = 17;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool wall =
                (x == 0 || y == 0 || x == width - 1 || y == height - 1) || // border
                (x % 8 == 0 && y % 8 == 0) ||                              // structured walls far apart
                ((x % 8 == 0 || y % 8 == 0) && dis(gen) < 0.08) ||         // 8% chance for grid connection
                (dis(gen) < 0.015);                                        // 1.5% chance for random

            if (!wall) continue;

            auto tex_ptr = brick;
            auto color   = COLOR::make_rgb(255, 255, 255);

            switch (tex(gen)) {
                case 0: tex_ptr = brick; color = COLOR::make_rgb(200, 80, 50); break;
                case 1: tex_ptr = wood;  color = COLOR::make_rgb(90, 70, 20);  break;
                case 2: tex_ptr = metal; color = COLOR::make_rgb(120, 120, 120); break;
            }

            auto entity = std::make_shared<ENTITY::TexturedMeshEntity>(mesh, tex_ptr, color);
            entity->set_position({ x - width / 2.0f, 0.0f, y - height / 2.0f });

            auto mesh_entity = std::dynamic_pointer_cast<ENTITY::MeshEntity>(entity);
            if (mesh_entity) {
                mesh_entity->set_scale({ 1.0f, 1.0f, 1.0f });
            }

            em.add_entity(entity);
        }
    }
}



int main() {
    try {
        CORE::Core engine({100, 50});
        engine.on_start = initialize_scene;
        engine.set_target_framerate(60);
        engine.initialize();
        engine.run_game_loop();
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
