#pragma once

#include "../utils/math.hpp"
#include "entity.hpp"

namespace ENTITY {
    using Vec3Float = MATH::Vec3<float>;

    class WorldEntity : public Entity {
    private:
        Vec3Float position; // Position of the entity in the world
    public:
        explicit WorldEntity() : Entity(), position(Vec3Float::zero()) {
            // Initialize position to origin
        }

        Vec3Float get_position() const {
            return position;
        }

        void set_position(const Vec3Float& new_position) {
            position = new_position;
        }
    };
}