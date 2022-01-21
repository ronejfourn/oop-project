#ifndef UTIL_H
#define UTIL_H
#include <cmath>
#include <cstdint>

constexpr float pi      = 3.14159265f;
constexpr float halfpi  =     pi / 2;
constexpr float thalfpi = 3 * pi / 2;
constexpr float tau     = 2 * pi;

#define ut_clamp(val, min, max) ((val) > (max)) ? (max) : ((val) < (min)) ? (min) : (val)

template <typename T>
struct __vec2 {
    T x, y;
    __vec2() {x = 0; y = 0;};
    __vec2(T vx, T vy) {x = vx; y = vy;};
    __vec2 operator +(const __vec2 &op) const {return {x + op.x, y + op.y};}
    __vec2 operator -(const __vec2 &op) const {return {x - op.x, y - op.y};}
    __vec2 operator *(const __vec2 &op) const {return {x * op.x, y * op.y};}
    __vec2 operator /(const __vec2 &op) const {return {x / op.x, y / op.y};}
    void operator +=(const __vec2 &op) {x += op.x; y += op.y;}
    void operator -=(const __vec2 &op) {x -= op.x; y -= op.y;}
    void operator *=(const __vec2 &op) {x *= op.x; y *= op.y;}
    void operator /=(const __vec2 &op) {x /= op.x; y /= op.y;}
    __vec2 operator *(const T &op) const {return {x * op, y * op};}
    __vec2 operator /(const T &op) const {return {x / op, y / op};}
    void operator *=(const T &op) {x *= op; y *= op;}
    void operator /=(const T &op) {x /= op; y /= op;}
    __vec2 perpendicular() const {return {-y, x};};
    void zero() {x = 0; y = 0;};
    float dot(const __vec2 &op) const {return x * op.y + y * op.x;};
    float magnitude() const {return sqrtf(x * x + y * y);};
    float angleBetn(const __vec2 &op) const {
        float dfx = op.x - x;
        float dfy = op.y - y;
        float ret = dfx ? std::atan(std::abs(dfy / dfx)) : pi / 2;
        if (dfx >= 0 && dfy >= 0)
            return ret;
        else if (dfx <= 0 && dfy >= 0)
            ret = pi - ret;
        else if (dfx <= 0 && dfy <= 0)
            ret = pi + ret;
        else if (dfx >= 0 && dfy <= 0)
            ret = tau - ret;
        return ret;
    }
};

typedef __vec2<float> Vec2f;
typedef __vec2<int> Vec2i;
#endif
