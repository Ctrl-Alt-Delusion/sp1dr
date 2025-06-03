#pragma once

#include <math.h>

namespace MATH {
    template<typename T>
    struct Vec2 {
        T x, y;

        T magnitude() const {
            return sqrt(x * x + y * y);
        }

        Vec2 normalize() const {
            const T m = magnitude();
            return { x / m, y / m };
        }

        inline Vec2 operator+(const Vec2& other) const {
            return {x + other.x, y + other.y};
        }

        inline T operator*(const Vec2& other) const {
            return x * other.x + y * other.y;
        }

        inline bool operator==(const Vec2& other) const {
            return (x == other.x && y == other.y);
        }
    };

    template<typename T>
    struct Vec3 {
        T x, y, z;

        T magnitude() const {
            return sqrt(x * x + y * y + z * z);
        }

        Vec3 normalize() const {
            const T m = magnitude();
            return { x / m, y / m, z / m };
        }

        Vec3 cross(const Vec3& other) const {
            return {y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x};
        }

        inline Vec3 operator+(const Vec3& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        inline T operator*(const Vec3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        inline bool operator==(const Vec3& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }
    };
}