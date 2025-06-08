// camera.hpp
#pragma once
#include "../math/math.hpp"

#include <vector>

#ifdef IN
#undef IN
#endif

#ifdef OUT
#undef OUT
#endif

namespace CORE {
    enum Direction { LEFT, RIGHT, UP, DOWN, IN, OUT };


    class Camera {
    public:
        Camera(bool /*dummy*/ = true) {}
        virtual ~Camera() = default;
    };

    class OrbitCamera : public Camera {
    public:
        // Keep track of yaw (rotation around Y) and pitch (rotation around X)
        float yaw   = 0.0f;   // left-right
        float pitch = 0.0f;   // up-down
        float dist  = 4.0f;   // camera distance from origin (you were using +4 before)

        // WASD/arrow keys drive these deltas in radians per keypress/frame
        float rot_angle_delta = 0.1f;

        // Keys: a/d → yaw, w/s → pitch
        std::vector<char> input_char {'a','d','w','s','z','x'};

        OrbitCamera(float initYaw = 0.0f, float initPitch = 0.0f, float radius = 4.0f)
            : yaw(initYaw), pitch(initPitch), dist(radius) {}

        // Adjust angles based on input
        void handle_movement(Direction direction);

        MATH::Vec3<float> world_to_view(const MATH::Vec3<float>& point) const {
            auto view = point;
            view = view.rotate_y(-yaw).rotate_x(-pitch);
            view.z += dist;
            return view;
        }

    };

}
