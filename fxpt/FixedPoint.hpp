#pragma once

#ifndef __INT24_TYPE__
#error "This library is intended for the 24-bit ez80 architecture"
#endif

#include <cstdio>
#include <cmath>

extern "C" {
    int __operator_mul(int a, int b);
    int __operator_div(int a, int b);
}

// 16.8 signed fixed point number
class FixedPoint {
    public:
    union {
        int t;
        struct {
            unsigned char f;
            short i;
        };
    };
    private:
    #define ISZ (8*sizeof(i))
    #define FSZ (8*sizeof(f))
    #define IMAX (1 << ISZ)
    #define FMAX (1 << FSZ)
    public:
    inline FixedPoint(float v) {
        i = floorf(v);
        f = (v - i) * FMAX;
    }
    inline FixedPoint(int vi=0, unsigned char vf=0) {
        i = vi;
        f = vf;
    }
    private:
    static inline FixedPoint fromInt(int i) {
        return *(FixedPoint*)&i;
    }
    public:
    inline FixedPoint operator+(FixedPoint o) {
        return fromInt(t+o.t);
    }
    inline FixedPoint operator-(FixedPoint o) {
        return fromInt(t-o.t);
    }
    inline FixedPoint operator*(FixedPoint o) {
        return fromInt(__operator_mul(t, o.t));
    }
    inline FixedPoint operator/(FixedPoint o) {
        return fromInt(__operator_div(t, o.t));
    }
    inline FixedPoint operator%(FixedPoint o) {
        return fromInt(t % o.t);
    }
    inline FixedPoint operator+=(FixedPoint o) {
        return (*this = *this + o);
    }
    inline FixedPoint operator-=(FixedPoint o) {
        return (*this = *this - o);
    }
    inline FixedPoint operator*=(FixedPoint o) {
        return (*this = *this * o);
    }
    inline FixedPoint operator/=(FixedPoint o) {
        return (*this = *this / o);
    }
    inline FixedPoint operator%=(FixedPoint o) {
        return (*this = *this % o);
    }
    inline FixedPoint operator++() {
        return *this = fromInt(*(int*)this + FMAX);
    }
    inline FixedPoint operator--() {
        return *this = fromInt(*(int*)this - FMAX);
    }
    inline operator int() {
        return i;
    }
    inline operator float() {
        return float(i) + float(f) * (1.0f / float(FMAX));
    }

    inline FixedPoint trunc() {
        return FixedPoint(i, 0);
    }
    inline FixedPoint floor() {
        return trunc();
    }
    inline FixedPoint ceil() {
        return FixedPoint(f>=FMAX/2 ? i+1 : i, 0);
    }
    inline FixedPoint round() {
        if (f < FMAX/2) {
            return floor();
        } else {
            return FixedPoint(i+1, 0);
        }
    }
    inline FixedPoint sqrt() {
        return FixedPoint(sqrtf(float(*this)));
    }
    inline FixedPoint pow2() {
        return fromInt(__operator_mul(t, t));
    }
    inline FixedPoint pow3() {
        return fromInt(__operator_mul(t, __operator_mul(t, t)));
    }
    inline FixedPoint pow(float exp) {
        return powf(float(*this), exp);
    }
    // note that powers higher than 4 will overflow and give incorrect results.
    inline FixedPoint powi(unsigned int exp) {
        // int r = t;
        // while (!(exp & 1)) {
        //     r = __operator_mul(r, r);
        //     exp >>= 1;
        // }
        int rv = t;
        for (unsigned int i=1; i<exp; i++) {
            rv = __operator_mul(rv, t);
        }
        return fromInt(rv);
    }
    // return a float string representation of this fixed point number.
    inline int tostring(char* buf) {
        return sprintf(buf, "%u.%03d", i, f*1000/256);
    }
    // parse a float string and return a fixed point number.
    static inline FixedPoint fromstring(char* str) {
        unsigned int u = 0;
        unsigned int l = 0;
        unsigned int n = 1;
        char c;
        bool neg = false;
        if (str[0] == '-') {
            neg = true;
            str++;
        }
        while ((c = *str++) && c != '.') {
            if (c >= '0' && c <= '9')
                u = u * 10 + c - '0';
        }
        if (c == '.') {
            while ((c = *str++)) {
                if (c >= '0' && c <= '9') {
                    l = l * 10 + c - '0';
                    n *= 10;
                }
            }
        }
        return FixedPoint(neg ? -u : u, (l*256)/n);
    }
};