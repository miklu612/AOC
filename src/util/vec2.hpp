#ifndef vec2_hpp
#define vec2_hpp

#include<cmath>
#include<iostream>

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

#endif