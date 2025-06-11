#pragma once

#include "../color/color.hpp"

#include <string>
#include <vector>

namespace TEXTURE {

class Texture {
protected:
    std::string texture_pattern;

public:
    Texture(std::string texture_pattern);
    virtual std::vector<std::string> generate_texture() = 0;
    virtual ~Texture() = default;
};

class Texture2D : protected Texture {
private:
    size_t width;
    size_t height;

public:
    Texture2D(size_t width, size_t height, std::string pattern);
    void set_dimensions(size_t width, size_t height);
    std::vector<std::string> generate_texture() override;
};

class ColoredTexture2D : public Texture2D {
private:
    std::vector<COLOR::RGBColor> color_pattern; // Pattern for colored textures
public:
    ColoredTexture2D(size_t width, size_t height, std::string texture_pattern, std::vector<COLOR::RGBColor> color_pattern)
        : Texture2D(width, height, std::move(texture_pattern)), color_pattern(std::move(color_pattern)) {}

    std::vector<COLOR::RGBColor> get_color_pattern() const {
        return color_pattern;
    }
};

}
