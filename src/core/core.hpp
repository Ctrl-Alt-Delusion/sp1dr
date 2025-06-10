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
#include <functional>

namespace CORE {
    extern char bg_char;
    extern ENTITY::EntityManager entity_manager;


    struct BarycentricCoords {
        float u, v, w;
        bool is_inside() const {
            return u >= 0 && v >= 0 && w >= 0;
        }
    };


    class Core {
    public:
        Core(pair_uint screen_size);
        void game_loop();
        
        void game_logic();
        void game_logic(OrbitCamera& camera);

        void init();

        std::function<void()> start       = nullptr; // Function to run at the start of the game
        std::function<void()> update      = nullptr; // Function to update game logic each frame
        std::function<void()> late_update = nullptr; // Function to run after all updates

        void set_target_frame_rate(size_t rate) {target_frame_rate = rate;}

    private:
        size_t   target_frame_rate = 60;
        Screen   screen;
        Renderer renderer;
    };

    char get_depth_shade(float z);

    //void draw_line(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1, CORE::Screen& screen, char pixel = '#');
    void draw_line_zbuffered(const MATH::Vec2<int>& p0, const MATH::Vec2<int>& p1,
                            float z0, float z1,
                            CORE::Screen& screen,
                            std::vector<std::vector<float>>& z_buffer,
                            char pixel);
    
} // namespace CORE
