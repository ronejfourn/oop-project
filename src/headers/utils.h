#ifndef UTIL_H
#define UTIL_H
#include <cstdint>

constexpr float pi = 3.14159265f;

#define ut_clamp(val, min, max) (((val) > (max)) ? (max) : ((val) < (min)) ? (min) : (val))
#define ut_abs(a) (((a) > (0)) ? (a) : -(a))
#define ut_ceil(a) (int(a) + 1)

static float Q_rsqrt(float number )
{
    int32_t i;
    float x2, y;
    const float threehalfs = 1.5f;
    x2 = number * 0.5f;
    y  = number;
    i  = *(int32_t*) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = *(float*)&i;
    y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
    //  y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}

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
    float dot(const __vec2 &op) const {return x * op.y + y * op.x;}
    float magnitude() const {
        float sqr = (x * x + y * y);
        return Q_rsqrt(sqr) * sqr;
    }
    __vec2 normalized() {
        float invmag = Q_rsqrt(x * x + y * y);
        return {x * invmag, y * invmag};
    }
};

typedef __vec2<float> Vec2f;
typedef __vec2<int> Vec2i;

template <typename T>
struct __rect {
    __vec2<T> pos, dim;
    __rect() {pos.x = 0; pos.y = 0; dim.x = 0; dim.y = 0;}
    __rect(T x, T y, T w, T h) {pos.x = x; pos.y = y; dim.x = w; dim.y = h;}
};

typedef __rect<float> Rectf;
typedef __rect<int> Recti;

#include <iostream>

namespace Logger {
    static void LogError(const char *msg, int err_code = -1) {
        std::cout << "[ERROR]: " << msg << std::endl;
        exit(err_code);
    }

    static void LogWarning(const char *msg) {
        std::cout << "[WARNING]: " << msg << std::endl;
    }
}

struct SDL_Texture;
struct Map {
    Vec2i *tilemap;
    int   *indices;
    Vec2i dim;
    int tilesize, drawsize;
    SDL_Texture *texture;
};

#endif
