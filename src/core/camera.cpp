#include "camera.hpp"

namespace CORE {
    void FirstPersonCamera::handle_movement(Direction direction) {
        // TODO: handle this elsewhere
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