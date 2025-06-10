#pragma once 

#include "../math/math.hpp"
#include "../meshes/meshes.hpp"
#include "../texture/texture.hpp"

#include <iostream>
#include <memory>

namespace ENTITY {

    class Entity {
    private:
        MATH::Vec3<float> position {0.0f, 0.0f, 0.0f}; // Default position at origin
    public:
        virtual ~Entity() = default;

        // Entities can have a position in 3D space
        virtual MATH::Vec3<float> get_position() const {
            return position;
        }

        virtual void set_position(const MATH::Vec3<float>& pos) {
            position = pos;
        }
    };

    class MeshEntity : public Entity {
    private:
        MATH::Vec3<float> scale {1.0f, 1.0f, 1.0f}; // Default scale of 1 in all dimensions

        MESHES::Mesh* mesh = nullptr; // Pointer to the mesh data
    public:
        MeshEntity(MESHES::Mesh* mesh_data) : mesh(mesh_data) {
            if (!mesh) {
                throw std::invalid_argument("Mesh data cannot be null");
            }
        }
        virtual ~MeshEntity() = default;

        // Entities can have a scale
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

    class TexturedMeshEntity : public MeshEntity {
    private:
        std::shared_ptr<TEXTURE::Texture2D> texture;
        std::vector<std::string> texture_data;
    public:
        TexturedMeshEntity(MESHES::Mesh* mesh_data, std::shared_ptr<TEXTURE::Texture2D> tex) 
            : MeshEntity(mesh_data), texture(tex) {
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
        
        // Get texture character at UV coordinates (0-1 range)
        char get_texture_char(float u, float v) const {
            if (texture_data.empty()) return '#';
            
            // Clamp UV coordinates
            u = std::max(0.0f, std::min(1.0f, u));
            v = std::max(0.0f, std::min(1.0f, v));
            
            // Convert to texture coordinates
            int tex_x = static_cast<int>(u * (texture_data[0].size() - 1));
            int tex_y = static_cast<int>(v * (texture_data.size() - 1));
            
            return texture_data[tex_y][tex_x];
        }
    };
} // namespace ENTITY