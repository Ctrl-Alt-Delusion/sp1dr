#pragma once

#include "../utils/math.hpp"
#include "../core/component.hpp"

// Most complex part of our codebase
namespace COMPONENTS {
    struct PositionComponent : public CORE::Component {
        MATH::Vec3<float> position;
    };

    struct VelocityComponent : public CORE::Component {
        MATH::Vec3<float> velocity;
    };
    
    struct ScaleComponent : public CORE::Component {
        MATH::Vec3<float> scale;
    };
} // namespace COMPONENTS