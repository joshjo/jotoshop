#ifndef PIXEL_H
#define PIXEL_H

#include <stdio.h>
#include <iomanip>
#include <iostream>

typedef int pint;

using namespace std;

static const int maxColor = 256;

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

    Pixel () {}

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
