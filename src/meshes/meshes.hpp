// Enhanced meshes.hpp - Add face support
#pragma once
#include "../math/math.hpp"
#include <vector>

namespace MESHES {
    // Triangle face structure
    typedef MATH::Vec3<size_t> Face;

    class Mesh {
    public:
        Mesh() = default;
        virtual ~Mesh() = default;
        virtual const std::vector<MATH::Vec3<float>>& getVertices() const = 0;
        virtual const std::vector<MATH::Vec2<size_t>>& getEdges() const = 0;
        virtual const std::vector<Face>& getFaces() const = 0;  // New method

        virtual const std::vector<MATH::Vec2<float>>& getUVs() const = 0;
    };

    class Cube : public Mesh {
    private:
        std::vector<MATH::Vec3<float>> vertices = {
            {-1, -1, -1},  // 0
            { 1, -1, -1},  // 1
            { 1,  1, -1},  // 2
            {-1,  1, -1},  // 3
            {-1, -1,  1},  // 4
            { 1, -1,  1},  // 5
            { 1,  1,  1},  // 6
            {-1,  1,  1}   // 7
        };

        std::vector<MATH::Vec2<size_t>> edges = {
            {0,1}, {1,2}, {2,3}, {3,0},  // Front face
            {4,5}, {5,6}, {6,7}, {7,4},  // Back face
            {0,4}, {1,5}, {2,6}, {3,7}   // Connecting edges
        };

        // Define faces as triangles (2 triangles per cube face)
        std::vector<Face> faces = {
            // Front face (-Z)
            Face{0, 1, 2}, Face{0, 2, 3},
            // Back face (+Z)
            Face{4, 7, 6}, Face{4, 6, 5},
            // Left face (-X)
            Face{0, 3, 7}, Face{0, 7, 4},
            // Right face (+X)
            Face{1, 5, 6}, Face{1, 6, 2},
            // Bottom face (-Y)
            Face{0, 4, 5}, Face{0, 5, 1},
            // Top face (+Y)
            Face{3, 2, 6}, Face{3, 6, 7}
        };

        std::vector<MATH::Vec2<float>> uvs = {
            // 8 UVs, one per vertex (you can tweak these as needed)
            {0.0f, 0.0f}, // vertex 0
            {1.0f, 0.0f}, // vertex 1
            {1.0f, 1.0f}, // vertex 2
            {0.0f, 1.0f}, // vertex 3
            {0.0f, 0.0f}, // vertex 4
            {1.0f, 0.0f}, // vertex 5
            {1.0f, 1.0f}, // vertex 6
            {0.0f, 1.0f}  // vertex 7
        };

    public:
        const std::vector<MATH::Vec3<float>>& getVertices() const override {
            return vertices;
        }

        const std::vector<MATH::Vec2<size_t>>& getEdges() const override {
            return edges;
        }

        const std::vector<Face>& getFaces() const override {
            return faces;
        }

        const std::vector<MATH::Vec2<float>>& getUVs() const override {
            return uvs;
        }

    };

    extern Cube static_cube_mesh;
}