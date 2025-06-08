#pragma once 

#include "../math/math.hpp"
#include "../meshes/meshes.hpp"

#include <iostream>

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
} // namespace ENTITY