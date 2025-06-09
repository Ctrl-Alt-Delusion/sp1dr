#pragma once 

#include "../math/math.hpp"
#include <vector>

namespace MESHES {
    class Mesh {
    public:
        Mesh() = default;
        virtual ~Mesh() = default;

        virtual const std::vector<MATH::Vec3<float>>& getVertices() const = 0;
        virtual const std::vector<MATH::Vec2<size_t>>& getEdges() const = 0;
    };

    class Cube : public Mesh {
    private:
        std::vector<MATH::Vec3<float>> vertices = {
            {-1, -1, -1},
            { 1, -1, -1},
            { 1,  1, -1},
            {-1,  1, -1},
            {-1, -1,  1},
            { 1, -1,  1},
            { 1,  1,  1},
            {-1,  1,  1}
        };

        std::vector<MATH::Vec2<size_t>> edges = {
            {0,1}, {1,2}, {2,3}, {3,0},
            {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
        };
        
    public:
        const std::vector<MATH::Vec3<float>>& getVertices() const override {
            return vertices;
        }

        const std::vector<MATH::Vec2<size_t>>& getEdges() const override {
            return edges;
        }
    };

    extern Cube static_cube_mesh;
}