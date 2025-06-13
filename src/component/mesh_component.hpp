#pragma once

#include "component.hpp"
#include "../utils/math.hpp"

#include <vector>

namespace COMPONENT {

    typedef MATH::Vec3<float> Vec3Float;
    typedef MATH::Vec2<int>   Vec2Int;
    typedef MATH::Vec2<float> Vec2Float;


    class Mesh : public Component {
    private:
    public:
        Mesh() = default;
        virtual ~Mesh() = default;
        virtual const std::vector<Vec3Float>& getVertices() const = 0;
        virtual const std::vector<Vec2Int>& getEdges() const = 0;
        virtual const std::vector<Vec3Float>& getFaces() const = 0;

        virtual const std::vector<Vec2Float>& getUVs() const = 0;
    };


    class CubeMesh : public Mesh {
        private:
        std::vector<Vec3Float> vertices = {
            {-1, -1, -1},  // 0
            { 1, -1, -1},  // 1
            { 1,  1, -1},  // 2
            {-1,  1, -1},  // 3
            {-1, -1,  1},  // 4
            { 1, -1,  1},  // 5
            { 1,  1,  1},  // 6
            {-1,  1,  1}   // 7
        };

        std::vector<Vec2Int> edges = {
            {0,1}, {1,2}, {2,3}, {3,0},  // Front face
            {4,5}, {5,6}, {6,7}, {7,4},  // Back face
            {0,4}, {1,5}, {2,6}, {3,7}   // Connecting edges
        };

        // Define faces as triangles (2 triangles per cube face)
        std::vector<Vec3Float> faces = {
            // Front face (-Z)
            Vec3Float{0, 1, 2}, Vec3Float{0, 2, 3},
            // Back face (+Z)
            Vec3Float{4, 7, 6}, Vec3Float{4, 6, 5},
            // Left face (-X)
            Vec3Float{0, 3, 7}, Vec3Float{0, 7, 4},
            // Right face (+X)
            Vec3Float{1, 5, 6}, Vec3Float{1, 6, 2},
            // Bottom face (-Y)
            Vec3Float{0, 4, 5}, Vec3Float{0, 5, 1},
            // Top face (+Y)
            Vec3Float{3, 2, 6}, Vec3Float{3, 6, 7}
        };

        std::vector<Vec2Float> uvs = {
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
        const std::vector<Vec3Float>& getVertices() const override {
            return vertices;
        }

        const std::vector<Vec2Int>& getEdges() const override {
            return edges;
        }

        const std::vector<Vec3Float>& getFaces() const override {
            return faces;
        }

        const std::vector<Vec2Float>& getUVs() const override {
            return uvs;
        }
    };

}