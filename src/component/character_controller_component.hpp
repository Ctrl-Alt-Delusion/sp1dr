#pragma once 

#include "component.hpp"

#include <vector>

namespace COMPONENT {

    const std::vector<char> default_keyboard_inputs {'w', 's', 'a', 'd', 'z', 'x'};

    class CharacterController : public Component {
    private:
        enum directions {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};
        std::vector<char> keyboard_inputs {};
    public:
        explicit CharacterController(std::vector<char> input_chs = default_keyboard_inputs) : keyboard_inputs(input_chs) {};
    };

}