// camera.hpp
#pragma once
#include "../math/math.hpp"

#include <vector>

// If this shit breaks my fellow windows gooners are fucked
// Mush1e: [re-enters the room] *laughs in Linux 🎩*
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

        float focal_length = 6.0f; // focal length for projection
    };

    class FirstPersonCamera : public Camera {
    public:
    //TODO: Make camera an entity component
        MATH::Vec3<float> position {0.0f, 0.0f, 0.0f}; // Default position at origin
        float yaw = 0.0f; // left-right rotation
        float pitch = 0.0f; // up-down rotation

        std::vector<char> input_char {'a','d','w','s','z','x'};

        FirstPersonCamera(float initYaw = 0.0f, float initPitch = 0.0f)
            : yaw(initYaw), pitch(initPitch) {}

        void handle_movement(Direction direction);

        MATH::Vec3<float> world_to_view(const MATH::Vec3<float>& point) const {
            auto view = point;
            view = view.rotate_y(-yaw).rotate_x(-pitch);
            return view;
        }

        void set_move_speed(float speed) {
            move_speed = speed;
        }

        void set_rot_speed(float speed) {
            rot_speed = speed;
        }
    private:
        float move_speed = 0.1f; // speed of movement
        float rot_speed = 0.05f; // speed of rotation
    };
}
