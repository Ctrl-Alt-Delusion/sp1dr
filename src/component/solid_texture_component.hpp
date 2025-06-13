#pragma once

#include "component.hpp"

#include <vector>

namespace COMPONENT {

    std::vector<char> default_char_map {'@', '&', '%', '$', '#', 'X', 'A', 'M', 'N', 'W', 'Q', 'R', 'O', 'o', 'a', 'h',
                                    'k', 'b', 'd', 'p', 'q', 'w', 'm', 'Z', 'Y', 'U', 'J', 'C', 'L', 'z', 'c', 'v',
                                    'u', 'n', 'x', 'r', 'j', 'f', 't', '/', '\\', '|', '(', ')', '1', '{', '}', '[',
                                    ']', '?', '-', '_', '+', '~', '<', '>', 'i', '!', 'l', 'I', ';', ':', ',', '"',
                                    '^', '`', '\'', '.', ' '};

    class SolidTextureComponent : public Component {
    private:
        std::vector<char> char_map {};
    public:
        explicit SolidTextureComponent(std::vector<char> chars = default_char_map) : char_map(chars) {};

        char get_char_at(uint idx) const{
            return this->char_map[idx];
        }

        void set_char_at(uint idx, char ch) {
            this->char_map[idx] = ch;
        }
    };

}