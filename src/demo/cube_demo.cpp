#include "../core/core.hpp"
#include "../math/math.hpp"

namespace CUBE_DEMO {
    class Cube {
    private:
        MATH::Vec3<size_t> vertices[8] = {
            {1, 1, 1},
            {-1, 1, 1},
            {-1, -1, 1},
            {1, -1, 1},
            {1, 1, -1},
            {1, -1, -1},
            {-1, -1, -1},
            {-1, -1, 1}
        };
    }
}