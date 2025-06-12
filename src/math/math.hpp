#pragma once

#include <math.h>

namespace MATH {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON = 1e-6f; // Small value for floating point comparisons
    constexpr float SQUARE_ROOT_2 = 1.41421356237f; // sqrt(2)
    constexpr float INVERSE_SQUARE_ROOT_2 = 0.70710678118f; // 1/sqrt(2)

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

        T dot(const Vec3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        inline Vec3 operator+(const Vec3& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        inline Vec3 operator+=(const Vec3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        inline Vec3 operator*(const int o) const {
            return {x * o, y * o, z * o};
        }

        inline Vec3 operator-(const Vec3& other) const {
            return {x - other.x, y - other.y, z - other.z};
        }

        inline Vec3 operator-=(const Vec3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        inline T operator*(const Vec3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        inline bool operator==(const Vec3& other) const {
            return (x == other.x && y == other.y && z == other.z);
        }

        Vec3 rotate_x(float angle_rad) {
            float cos_a = cos(angle_rad);  
            float sin_a = sin(angle_rad);
            return {
                x,                          
                y * cos_a - z * sin_a,    
                z * cos_a + y * sin_a
            };
        }

        Vec3 rotate_y(float angle_rad) {
            float cos_a = cos(angle_rad);  
            float sin_a = sin(angle_rad);
            return {
                x * cos_a + z * sin_a,
                y,
                z * cos_a - x * sin_a,
            };
        }

        Vec3 rotate_z(float angle_rad) {
            float cos_a = cos(angle_rad);  
            float sin_a = sin(angle_rad);
            return {
                x * cos_a - y * sin_a,
                x * sin_a + y * cos_a, 
                z 
            };
        }

        Vec2<T> project(float focal_length = 1.0f) const{
            if (z == 0) return {x, y}; 
            return {
                (focal_length * x) / z,
                (focal_length * y) / z
            };
        }

        Vec3<T> operator-() const {
            return {-x, -y, -z};
        }

        Vec3<T> scale(Vec3<T> scale_factor) const {
            return {x * scale_factor.x, y * scale_factor.y, z * scale_factor.z};
        }
    };

    template<typename T>
    struct Vec4 {
        T x, y, z, w;

        Vec4<T> operator+(const Vec4<T>& other) const {
            return {x + other.x, y + other.y, z + other.z, w + other.w};
        }

        Vec4<T> operator*(const T scalar) const {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        Vec4<T> operator-(const Vec4<T>& other) const {
            return {x - other.x, y - other.y, z - other.z, w - other.w};
        }
    };
}

