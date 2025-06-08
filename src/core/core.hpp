#pragma once

#include "screen.hpp"
#include "renderer.hpp"
#include "camera.hpp"
#include "../meshes/meshes.hpp"
#include "input_handling.hpp"
#include "../entity/entity.hpp"
#include "../entity/entity_manager.hpp"

#include <algorithm>
#include <thread>
#include <chrono>

namespace CORE {

    extern MESHES::Cube cube;
    class Core {
    public:
        Core(pair_uint screen_size);          
        void game_loop();
        
        void game_logic();
        void game_logic(OrbitCamera& camera, ENTITY::EntityManager& entity_manager);

        void init();

    private:
        Screen   screen;
        Renderer renderer;
    };

    void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel = '#');
    void draw_line_zbuffered(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1,
                            float z0, float z1,
                            CORE::Screen& screen,
                            std::vector<std::vector<float>>& z_buffer,
                            char pixel);
    

} // namespace CORE
