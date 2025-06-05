#include "camera.hpp"
#include <algorithm>

namespace CORE {
    void OrbitCamera::handle_movement(Direction direction) {
        switch (direction) {
            case LEFT:  yaw   -= rot_angle_delta; break;
            case RIGHT: yaw   += rot_angle_delta; break;
            case UP:    pitch += rot_angle_delta; break;
            case DOWN:  pitch -= rot_angle_delta; break;
            case IN:    dist -= 1.0f; break;
            case OUT:   dist += 1.0f; break;
            default: break;
        }
        // Clamp pitch so the camera doesn’t flip over top/bottom
        if (pitch >  1.5f) pitch =  1.5f;  // ~≈ 85°
        if (pitch < -1.5f) pitch = -1.5f;  // ~≈ -85°
        
    }
}