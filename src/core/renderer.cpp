#include "renderer.hpp"

#include <iostream>

namespace CORE {

    Renderer::Renderer(Screen& screen_ref)
        : screen(screen_ref) {}

    void Renderer::render() {
        screen.clear_screen();

        auto buffer = screen.get_screen_buffer();
        auto color_buffer = screen.get_color_buffer();
        for (const auto &row : buffer) {
            for (int x = 0; x < row.size(); ++x) {
                const char pixel = row[x];
                if (pixel != ' ') {
                    std::cout << COLOR::RGB_TO_ANSI_STRING(
                        color_buffer[&row - &buffer[0]][x].x,
                        color_buffer[&row - &buffer[0]][x].y,
                        color_buffer[&row - &buffer[0]][x].z
                    ) << pixel;
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }
    }

} // CORE