#include "./core/core.hpp"

#include <iostream>

int main() {
    std::cout << "We live bois!" << std::endl;
    
    auto core = CORE::Core();
    core.game_loop();
    return 0;
}