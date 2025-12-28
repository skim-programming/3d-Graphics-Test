#pragma once
#include <iostream>
#include <cmath>
#include <type_traits>
#include <string>

class Vector2d {
public:
    float x, y;

    Vector2d() : x(0), y(0) {}

    Vector2d(float x_, float y_) : x(x_), y(y_) {}

    template <typename... Args>
        requires (sizeof...(Args) == 2) &&
    (std::is_convertible_v<Args, float> && ...)
        Vector2d(Args... args) {
        float tmp[2] = { static_cast<float>(args)... };
        x = tmp[0];
        y = tmp[1];
    }

    Vector2d operator+(const Vector2d& other) const {
        return Vector2d(x + other.x, y + other.y);
    }
    Vector2d& operator+=(const Vector2d& other) {
        x += other.x; y += other.y;
        return *this;
    }

    Vector2d& operator+=(float scalar) {
        x += scalar; y += scalar;
		return *this;
    }

    Vector2d operator-(const Vector2d& other) const {
        return Vector2d(x - other.x, y - other.y);
    }
    Vector2d& operator-=(const Vector2d& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    Vector2d operator*(float scalar) const {
        return Vector2d(x * scalar, y * scalar);
    }
    Vector2d& operator*=(float scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }

    Vector2d operator/(float scalar) const {
        return Vector2d(x / scalar, y / scalar);
    }
    Vector2d& operator/=(float scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }

    Vector2d operator-() const {
        return Vector2d(-x, -y);
    }

    bool operator==(const Vector2d& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2d& other) const {
        return !(*this == other);
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }
    float lengthSquared() const {
        return x * x + y * y;
    }

    Vector2d normalized() const {
        float len = length();
        return len == 0 ? Vector2d(0, 0) : Vector2d(x / len, y / len);
    }
    void normalize() {
        float len = length();
        if (len != 0) { x /= len; y /= len; }
    }

    float dot(const Vector2d& other) const {
        return x * other.x + y * other.y;
    }

    void print() const { // logging function
        std::cout << "x: " << x << "\ny: " << y << "\n\n";
    }
    std::string prints() const {
        return "x: " + std::to_string(x) + "\ny: " + std::to_string(y) + "\n\n";
    }
};

inline Vector2d operator*(float s, const Vector2d& v) {
    return Vector2d(v.x * s, v.y * s);
}

class Vector3d {
public:
    float x, y, z;

    Vector3d() : x(0), y(0), z(0) {}
    Vector3d(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    template <typename... Args>
        requires (sizeof...(Args) == 3) &&
    (std::is_convertible_v<Args, float> && ...)
        Vector3d(Args... args) {
        float tmp[3] = { static_cast<float>(args)... };
        x = tmp[0]; y = tmp[1]; z = tmp[2];
    }

    Vector3d operator+(const Vector3d& other) const {
        return Vector3d(x + other.x, y + other.y, z + other.z);
    }
    Vector3d& operator+=(const Vector3d& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vector3d operator-(const Vector3d& other) const {
        return Vector3d(x - other.x, y - other.y, z - other.z);
    }
    Vector3d& operator-=(const Vector3d& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vector3d operator*(float scalar) const {
        return Vector3d(x * scalar, y * scalar, z * scalar);
    }
    Vector3d& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vector3d operator/(float scalar) const {
        return Vector3d(x / scalar, y / scalar, z / scalar);
    }
    Vector3d& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    Vector3d operator-() const {
        return Vector3d(-x, -y, -z);
    }

    bool operator==(const Vector3d& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    bool operator!=(const Vector3d& other) const {
        return !(*this == other);
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3d normalized() const {
        float len = length();
        return len == 0 ? Vector3d(0, 0, 0) : Vector3d(x / len, y / len, z / len);
    }
    void normalize() {
        float len = length();
        if (len != 0) { x /= len; y /= len; z /= len; }
    }

    float dot(const Vector3d& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3d cross(const Vector3d& other) const {
        return Vector3d(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    void print() const { // logging function
        std::cout << "x: " << x << "\ny: " << y << "\nz: " << z << "\n\n";
    }
    std::string prints() const {
        return "x: " + std::to_string(x) + "\ny: " + std::to_string(y) + "\nz: " + std::to_string(z) + "\n\n";
    }
};

inline Vector3d operator*(float s, const Vector3d& v) {
    return Vector3d(v.x * s, v.y * s, v.z * s);
}

