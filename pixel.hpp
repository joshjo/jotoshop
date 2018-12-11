#ifndef PIXEL_H
#define PIXEL_H

#include <stdio.h>
#include <iomanip>
#include <iostream>

typedef int pint;

using namespace std;

static const int maxColor = 256;

static double Max(double a, double b) {
    return a >= b ? a : b;
}

static double Min(double a, double b) {
    return a <= b ? a : b;
}


struct Pixel {

    static const int B32 = 4;
    static const int B8 = 1;
    static const int B24 = 3;

    static const pint R = 0;
    static const pint G = 1;
    static const pint B = 2;
    static const pint A = 3;
    static const pint X = 0;

    pint * val;

    char mode;

    Pixel (const Pixel & other) {
        this->mode = other.mode;

        if (mode == B32) {
            val = new int[4];
            val[0] = other.val[0];
            val[1] = other.val[1];
            val[2] = other.val[2];
            val[3] = other.val[3];
        } else if (mode == B24) {
            val = new int[3];
            val[0] = other.val[0];
            val[1] = other.val[1];
            val[2] = other.val[2];
        } else if (mode == B8) {
            val = new int[1];
            val[0] = other.val[0];
        }
    }

    Pixel(char mode, pint r = 0, pint g = 0, int b = 0, int a = 0) {
        this->mode = mode;
        if (mode == B32) {
            val = new int[4];
            val[0] = r;
            val[1] = g;
            val[2] = b;
            val[3] = a;
        } else if (mode == B24) {
            val = new int[3];
            val[0] = r;
            val[1] = g;
            val[2] = b;
        } else if (mode == B8) {
            val = new int[1];
            val[0] = r;
        }
    }

    friend Pixel operator+(const Pixel& a, const Pixel& b) {
        Pixel px(a.mode);
        for (int i = 0; i < a.mode; i++) {
            px.val[i] = a.val[i] + b.val[i];
        }
        return px;
    }

    friend Pixel operator-(const Pixel& a, const Pixel& b) {
        Pixel px(a.mode);
        for (int i = 0; i < a.mode; i++) {
            px.val[i] = a.val[i] - b.val[i];
        }
        return px;
    }


    friend Pixel operator*(float scalar, const Pixel & a) {
        Pixel px(a.mode);
        for (int i = 0; i < a.mode; i++) {
            px.val[i] = int(scalar * a.val[i]);
        }
        return px;
    }

    Pixel interpolate(const Pixel & other) {
        Pixel newPixel(*this);
        for (int i = 0; i < mode; i++) {
            newPixel.val[i] = (other.val[i] + val[i]) / 2;
        }
        return newPixel;
    }

    void increaseColor(int c, pint increase) {
        updateColor(c, val[c] + increase);
    }

    void updateColor(int c, pint newVal) {
        val[c] = newVal % maxColor;
    }

    int averageColor() {
        int sum = 0;
        for (int i = 0; i < mode; i++) {
            sum += val[i];
        }
        return sum / mode;
    }

    pint * hsv() {
        pint min = Min(Min(val[R], val[G]), val[B]);
        pint max = Max(Max(val[R], val[G]), val[B]);
        pint * hsv = new pint[3];

        hsv[2] = max;
        pint h, s;

        if (hsv[2] == 0) {
            hsv[0] = 0;
            hsv[1] = 0;
            return hsv;
        }
        hsv[1] = 255 * (max - min) / hsv[2];
        if (hsv[1] == 0) {
            hsv[0] = 0;
            return hsv;
        }
        if (max == val[R]) {
            hsv[0] = 0 + 43 * (val[G] - val[B]) / (max - min);
        } else if (max == val[G]) {
            hsv[0] = 85 + 43 * (val[B] - val[R]) / (max - min);
        } else {
            hsv[0] = 171 + 43 * (val[R] - val[G]) / (max - min);
        }
        return hsv;
    }

    double * cmyk() {
        double dr = (double)val[R] / 255;
        double dg = (double)val[G] / 255;
        double db = (double)val[B] / 255;
        double k = 1 - Max(Max(dr, dg), db);
        double cyan = (1 - dr - k) / (1 - k);
        double magenta = (1 - dg - k) / (1 - k);
        double yellow = (1 - db - k) / (1 - k);

        double * value = new double[4];
        value[0] = cyan;
        value[1] = magenta;
        value[2] = yellow;
        value[3] = k;
        return value;
    }

    friend ostream& operator<< (ostream& os, const Pixel & pixel) {
        if (pixel.mode == B32) {
            os << "rgba(" << pixel.val[0] << ' ' << pixel.val[1] << ' ' << pixel.val[2] << ' ' << pixel.val[3] << ')';
        }
        else if (pixel.mode == B24) {
            os << "rgb(" << pixel.val[0] << ' ' << pixel.val[1] << ' ' << pixel.val[2] << ')';
        }
        else if (pixel.mode == B8) {
            os << "g(" << pixel.val[0] << ')';
        }
    }

};

#endif // PIXEL_H
