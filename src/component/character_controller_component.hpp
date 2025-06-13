#pragma once 

#include "component.hpp"
#include "../utils/types.hpp"

#include <vector>

namespace COMPONENT {

    const std::vector<char> default_keyboard_inputs {'w', 's', 'a', 'd', 'z', 'x'};

    class CharacterController : public Component {
    private:
        std::vector<char> keyboard_inputs {};
        UTILS::directions directions {};
    public:
        explicit CharacterController(std::vector<char> input_chs = default_keyboard_inputs) : keyboard_inputs(input_chs) {};

        UTILS::directions get_directions(char input_ch) {
            switch(input_ch) {
                case 'w': 
                    return UTILS::directions::FORWARD;
                    break;
                case 's': 
                    return UTILS::directions::BACKWARD;
                    break;
                case 'a': 
                    return UTILS::directions::LEFT;
                    break;
                case 'd': 
                    return UTILS::directions::RIGHT;
                    break;
                case 'z': 
                    return UTILS::directions::UP;
                    break;
                case 'x': 
                    return UTILS::directions::DOWN;
                    break;
                default:  
                    return UTILS::directions::INVALID;
                    break;
            }
        };


    };

}