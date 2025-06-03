#include "renderer.hpp"

#include <iostream>

namespace CORE {

    Renderer::Renderer(Screen& screen_ref)
        : screen(screen_ref) {}

    void Renderer::render() {
        screen.clear_screen();

        auto buffer = screen.get_screen_buffer();
        for (const auto &row : buffer) {
            for (const auto &ch : row) {
                std::cout << ch;
            }
            std::cout << '\n';
        }
    }

} // CORE