#pragma once
#include "../utils/math.hpp"
#include "entity.hpp"

namespace ENTITY {
    using Vec3Float = MATH::Vec3<float>;

    class CameraEntity : public Entity {
    private:
        Vec3Float position; // Position of the camera in the world
        float rotation_angle_y; // Rotation angle of the camera

    public:
        explicit CameraEntity() : Entity(), position(Vec3Float::zero()), rotation_angle_y(0.0f) {
            // Initialize position to origin
        }

        Vec3Float get_position() const {
            return position;
        }

        void set_position(const Vec3Float& new_position) {
            position = new_position;
        }

        float get_rotation_angle_y() const {
            return rotation_angle_y;
        }

        void set_rotation_angle_y(float angle) {
            rotation_angle_y = angle;
        }
    };
}