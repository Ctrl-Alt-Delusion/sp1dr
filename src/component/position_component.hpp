#pragma once

#include "component.hpp"
#include "../utils/math.hpp"


namespace COMPONENT {

    typedef MATH::Vec3<float> Vec3Float;

    class PositionComponent : public Component {
    private:
        Vec3Float position {MATH::Vec3<float>::zero()};
    public:
        explicit PositionComponent(Vec3Float pos) : position(pos) {};

        Vec3Float get_position() const {
            return this->position;
        }

        void set_position(Vec3Float new_pos) {
            this->position = new_pos;
        }
    };
}