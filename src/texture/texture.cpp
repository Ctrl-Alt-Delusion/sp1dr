#include "texture.hpp"

namespace TEXTURE {

    Texture::Texture(std::string texture_pattern)
        : texture_pattern(std::move(texture_pattern)) {}

    Texture2D::Texture2D(size_t width, size_t height, std::string pattern)
        : Texture(std::move(pattern)), width(width), height(height) {}

    void Texture2D::set_dimensions(size_t width, size_t height) {
        this->width = width;
        this->height = height;
    }

    std::vector<std::string> Texture2D::generate_texture() {
        std::vector<std::string> texture;
        size_t pattern_size = texture_pattern.size();
        size_t pattern_idx = 0;

        for (size_t i = 0; i < height; ++i) {
            std::string line;
            for (size_t j = 0; j < width; ++j) {
                line.push_back(texture_pattern[pattern_idx % pattern_size]);
                ++pattern_idx;
            }
            texture.push_back(std::move(line));
        }

        return texture;
    }

}
