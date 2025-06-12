#pragma once
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

class Texture2D : public Texture {
private:
    size_t width;
    size_t height;

public:
    Texture2D(size_t width, size_t height, std::string pattern);
    void set_dimensions(size_t width, size_t height);
    std::vector<std::string> generate_texture() override;
    
    size_t get_width() const { return width; }
    size_t get_height() const { return height; }
};

} // namespace TEXTURE