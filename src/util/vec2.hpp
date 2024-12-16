#ifndef vec2_hpp
#define vec2_hpp

#include<cmath>
#include<iostream>
#include<array>

#include"direction.hpp"

template<typename T>
class Vec2 {
    public:

        Vec2(T x, T y) : x(x), y(y) {
        }

        Vec2() : x(0), y(0) {
        }

        T x = 0;
        T y = 0;

        constexpr T distance_to(const Vec2<T> vector) const {
            return 
                std::sqrt(
                    std::pow(vector.x - x, 2) +
                    std::pow(vector.y - y, 2)
                );
        }

        constexpr Vec2<T> abs() const {
            return Vec2<T>(
                std::abs(x),
                std::abs(y)
            );
        }

        Vec2<float> normalize() const {

            const float float_x = (float) x;
            const float float_y = (float) y;
            const float length = std::sqrt(
                std::pow(float_x, 2) +
                std::pow(float_y, 2)
            );

            return Vec2<float>(
                float_x / length,
                float_y / length
            );
            
        }

        constexpr bool within_box(T w, T h) const {
            return
                x >= (T)0 && x < w &&
                y >= (T)0 && y < h;
        }

        constexpr bool within_box(const Vec2<T>& box) const {
            return
                x >= (T)0 && x < box.x &&
                y >= (T)0 && y < box.y;
        }

        void operator+=(const Vec2<T>& vector) {
            x += vector.x;
            y += vector.y;
        }

        constexpr Direction get_direction() const {
            if(x == 0 && y != 0) {
                if(y > 0) {
                    return Direction::Up;
                }
                else {
                    return Direction::Down;
                }
            }
            else if(x != 0 && y == 0) {
                if(x > 0) {
                    return Direction::Right;
                }
                else {
                    return Direction::Left;
                }
            }
            else {
                throw std::exception("Directions are only supported for axis wise directions");
            }
        }

        friend Vec2<T> operator/(const Vec2<T>& l, const T& r) {
            return Vec2<T>(l.x / r, l.y / r);
        }

        friend Vec2<T> operator*(const Vec2<T>& l, const T& r) {
            return Vec2<T>(l.x * r, l.y * r);
        }

        friend Vec2<T> operator+(const Vec2<T>& l, const Vec2<T>& r) {
            return Vec2<T>(l.x + r.x, l.y + r.y);
        }

        friend Vec2<T> operator-(const Vec2<T>& l, const Vec2<T>& r) {
            return Vec2<T>(l.x - r.x, l.y - r.y);
        }

        friend bool operator==(const Vec2<T>& l, const Vec2<T>& r) {
            return 
                l.x == r.x &&
                l.y == r.y;
        }

        friend std::ostream& operator<<(std::ostream& ostream, const Vec2<T>& vector) {
            ostream << "(" << vector.x << ", " << vector.y << ")";
            return ostream;
        }

        friend bool operator!=(const Vec2<T>& l, const Vec2<T>& r) {
            return !(l == r);
        }

};

template<typename T>
constexpr std::array<Vec2<T>, 4> create_direction_vectors() {
    return {
        Vec2<T>(0, 1),
        Vec2<T>(0, -1),
        Vec2<T>(1, 0),
        Vec2<T>(-1, 0),
    };
}

#endif