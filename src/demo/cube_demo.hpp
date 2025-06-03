#pragma once

#include "../math/math.hpp"

namespace CUBE_DEMO {
    struct Cube {
        
        std::vector<MATH::Vec3<float>> vertices = {
            {-1, -1, -1},
            { 1, -1, -1},
            { 1,  1, -1},
            {-1,  1, -1},
            {-1, -1,  1},
            { 1, -1,  1},
            { 1,  1,  1},
            {-1,  1,  1}
        };

        std::vector<MATH::Vec2<size_t>> edges = {
            {0,1}, {1,2}, {2,3}, {3,0},
            {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
        };

    };
}