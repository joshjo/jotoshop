#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "pixel.hpp"

struct Histogram {
    float * mat;

    Histogram() {
        mat = new float [maxColor];
        for (int j = 0; j < maxColor; j++) {
            mat[j] = 0;
        }
    }

    pint * limits() {
        pint * resp = new pint [2];
        bool isLeft = true;
        bool isRight = false;
        for (int j = 0; j < maxColor; j++) {
            if (isLeft) {
                if (mat[j]) {
                    isLeft = false;
                    isRight = true;
                    resp[1] = j;
                } else {
                    resp[0] = j;
                }
            }
            if (isRight) {
                if (mat[j]) {
                    resp[1] = j;
                }
            }
        }
        return resp;
    }

    void print() {
        for (int j = 0; j < maxColor; j++) {
            cout << mat[j] << ' ';
        }
        cout << endl;
    }
};

#endif // HISTOGRAM_H
