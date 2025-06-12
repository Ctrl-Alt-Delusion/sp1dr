#pragma once
#include "../math/math.hpp"
#include "../meshes/meshes.hpp"
#include "../texture/texture.hpp"
#include "../color/color.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace ENTITY {

class Entity {
private:
    MATH::Vec3<float> position {0.0f, 0.0f, 0.0f};
    COLOR::RGBColor base_color = COLOR::make_rgb(255, 255, 255); // Default white color
public:
    virtual ~Entity() = default;
    
    virtual MATH::Vec3<float> get_position() const {
        return position;
    }
    
    virtual void set_position(const MATH::Vec3<float>& pos) {
        position = pos;
    }
    
    // Color methods for all entities
    virtual COLOR::RGBColor get_color() const {
        return base_color;
    }
    
    virtual void set_color(const COLOR::RGBColor& color) {
        base_color = color;
    }
};

class MeshEntity : public Entity {
private:
    MATH::Vec3<float> scale {1.0f, 1.0f, 1.0f};
    MESHES::Mesh* mesh = nullptr;
public:
    MeshEntity(MESHES::Mesh* mesh_data) : mesh(mesh_data) {
        if (!mesh) {
            throw std::invalid_argument("Mesh data cannot be null");
        }
    }
    
    virtual ~MeshEntity() = default;
    
    virtual MATH::Vec3<float> get_scale() const {
        return scale;
    }
    
    virtual void set_scale(const MATH::Vec3<float>& new_scale) {
        scale = new_scale;
    }
    
    virtual MESHES::Mesh* get_mesh() const {
        return mesh;
    }
};

// Simple textured entity with color
class TexturedMeshEntity : public MeshEntity {
private:
    std::shared_ptr<TEXTURE::Texture2D> texture;
    std::vector<std::string> texture_data;
    COLOR::RGBColor texture_color = COLOR::make_rgb(255, 255, 255); // White by default
    
public:
    TexturedMeshEntity(MESHES::Mesh* mesh_data, std::shared_ptr<TEXTURE::Texture2D> tex)
        : MeshEntity(mesh_data), texture(tex) {
        if (texture) {
            texture_data = texture->generate_texture();
        }
    }
    
    // Constructor with color
    TexturedMeshEntity(MESHES::Mesh* mesh_data, std::shared_ptr<TEXTURE::Texture2D> tex, const COLOR::RGBColor& color)
        : MeshEntity(mesh_data), texture(tex), texture_color(color) {
        if (texture) {
            texture_data = texture->generate_texture();
        }
    }
    
    virtual ~TexturedMeshEntity() = default;
    
    std::shared_ptr<TEXTURE::Texture2D> get_texture() const {
        return texture;
    }
    
    const std::vector<std::string>& get_texture_data() const {
        return texture_data;
    }
    
    // Set the color for the entire texture
    void set_texture_color(const COLOR::RGBColor& color) {
        texture_color = color;
    }
    
    COLOR::RGBColor get_texture_color() const {
        return texture_color;
    }
    
    // Get texture character at UV coordinates
    char get_texture_char_at_uv(float u, float v) const {
        if (texture_data.empty()) return '#';
        
        u = std::max(0.0f, std::min(1.0f, u));
        v = std::max(0.0f, std::min(1.0f, v));
        
        size_t tex_x = static_cast<size_t>(u * (texture_data[0].size() - 1));
        size_t tex_y = static_cast<size_t>(v * (texture_data.size() - 1));
        
        return texture_data[tex_y][tex_x];
    }
    
    // Combined texture and color sample
    struct TextureSample {
        char character;
        COLOR::RGBColor color;
    };
    
    TextureSample get_texture_sample(float u, float v) const {
        return {get_texture_char_at_uv(u, v), texture_color};
    }
};

} // namespace ENTITY