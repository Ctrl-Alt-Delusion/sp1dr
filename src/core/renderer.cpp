#include "renderer.hpp"
#include <iostream>
#include <sstream>

namespace CORE {
    Renderer::Renderer(Screen& screen_ref)
        : screen(screen_ref) {}
    
    void Renderer::render() {
        screen.clear_screen();
        const auto& buffer = screen.get_screen_buffer();
        const auto& color_buffer = screen.get_color_buffer();

        for (size_t y = 0; y < buffer.size(); ++y) {
            std::ostringstream row_stream;
            auto last_color = COLOR::BLACK;
            bool color_active = false;

            const auto& row = buffer[y];
            const auto& color_row = color_buffer[y];

            for (size_t x = 0; x < row.size(); ++x) {
                const char pixel = row[x];

                if (pixel != ' ') {
                    const auto& color = color_row[x];
                    if (!color_active || color.x != last_color.x || color.y != last_color.y || color.z != last_color.z) {
                        row_stream << COLOR::RGB_TO_ANSI_STRING(color.x, color.y, color.z);
                        last_color = color;
                        color_active = true;
                    }
                    row_stream << pixel;
                    continue;
                }
                
                if (color_active) {
                    row_stream << COLOR::ANSI_RESET;
                    color_active = false;
                }
                row_stream << ' ';
            }

            if (color_active) {
                row_stream << COLOR::ANSI_RESET;
            }

            std::cout << row_stream.str() << '\n';
        }
    }

    //danqzq: [spawns in] little did I know how offscreen I was..
}
