#include "camera.hpp"

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

    void FirstPersonCamera::handle_movement(Direction direction) {
        const float move_speed = 0.5f;  // how far to move per step
        const float rot_speed = 0.1f;   // how much to rotate per step (radians)

        switch (direction) {
            case LEFT:
                yaw -= rot_speed;  // rotate left
                break;
            case RIGHT:
                yaw += rot_speed;  // rotate right
                break;
            case UP: {
                // Move forward in the direction camera is facing on XZ plane
                position.x -= move_speed * sin(yaw);
                position.z -= move_speed * cos(yaw);
                break;
            }
            case DOWN: {
                // Move backward
                position.x += move_speed * sin(yaw);
                position.z += move_speed * cos(yaw);
                break;
            }
            default:
                break;
        }
    }


}