#ifndef IMAGE_H
#define IMAGE_H

#include "pixel.hpp"
#include "histogram.hpp"


using namespace std;


class Image {
    void init (int mode, int width, int height) {
        this->height = height;
        this->width = width;
        this->mode = mode;

        matrix = new Pixel ** [height];
        for (int i = 0; i < height; ++i) {
            matrix[i] = new Pixel * [width];
            for (int j = 0; j < width; j++) {
                matrix[i][j] = new Pixel(mode);
            }
        }
    }
public:
    Pixel *** matrix;
    pint height;
    pint width;
    int mode;

    Image(int mode, int width, int height) {
        init(mode, width, height);
    }

    Image(char const * filename) {
        int i;
        FILE* f = fopen(filename, "rb");

        if(f == NULL) {
            throw "Argument Exception";
        }

        unsigned char info[54];
        int HEADER_SIZE = 54;
        fread(info, sizeof(unsigned char), HEADER_SIZE, f); // read the 54-byte header

        // extract image height and width from header
        int dataOffset = *(int*)&info[10];
        int width = *(int*)&info[18];
        int height = *(int*)&info[22];

        init(Pixel::B24, width, height);
        cout << "width: " << width << endl;
        cout << "height: " << height << endl;

        int row_padded = (width*3 + 3) & (~3);
        unsigned char* data = new unsigned char[row_padded];

        fread(data, sizeof(unsigned char), dataOffset - HEADER_SIZE, f);

        for(int i = 0; i < height; i++)
        {
            fread(data, sizeof(unsigned char), row_padded, f);
            for(int j = 0; j < width*3; j += 3)
            {
                setPixel(
                    i, j / 3,
                    (int) data[j + 2],
                    (int) data[j + 1],
                    (int) data[j]
                );
            }
        }

        fclose(f);
    }

    void setPixel(int i, int j, int r = -1, int g = -1, int b = -1, int a = -1) {
        if (r >= 0) {
            matrix[i][j]->val[Pixel::R] = r;
        }
        if (g >= 0) {
            matrix[i][j]->val[Pixel::G] = g;
        }
        if (b >= 0) {
            matrix[i][j]->val[Pixel::B] = b;
        }
        if (a >= 0) {
            matrix[i][j]->val[Pixel::A] = a;
        }
    }

    Histogram * histogram() {
        float d = 1.0 / height / width;
        Histogram * hist = new Histogram[mode];
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                for (int k = 0; k < mode; k++) {
                    hist[k].mat[matrix[i][j]->val[k]]  += d;
                }
            }
        }

        return hist;
    }

    void equalize() {

        Histogram * currentHistogram = histogram();
        float ** lookup = new float * [mode];
        for (int i = 0; i < mode; i++) {
            lookup[i] = new float [maxColor];
        }
        for (int i = 0; i < mode; i++) {
            Histogram hist = currentHistogram[i];
            float sum = 0;

            for (int j = 0; j < maxColor; j++) {
                sum += hist.mat[j];
                lookup[i][j] = sum * 255 + 0.5;
            }
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                for (int k = 0; k < mode; k++) {
                    matrix[i][j]->updateColor(k, lookup[k][matrix[i][j]->val[k]]);
                }
            }
        }
    }

    void threshold(int limit = 128) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                pint color = 0;
                int avg = matrix[i][j]->averageColor();
                // cout << "avg: " << avg << " - ";
                if (avg > limit) {
                    color = 255;
                }
                for (int k = 0; k < mode; k++) {
                    matrix[i][j]->updateColor(k, color);
                }
            }
        }
    }

    void colorTransformation(int c, pint increase) {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                if (matrix[i][j] != NULL) {
                    matrix[i][j]->increaseColor(c, increase);
                }
            }
        }
    }

    void convolution(int kernelSize, int ** kernel) {
        int kernelArea = kernelSize * kernelSize;

        int delta = kernelSize / 2;

        int sum_position = 0;
        size_t begin = delta;
        size_t i, j;
        int a, b;
        Image * convoluted = new Image(mode, width, height);

        for (i = begin; i < height - delta; ++i) {
            for(j = begin; j < width - delta; ++j) {
                for (int k = 0; k < mode; k++) {
                    for (a = -delta; a < delta; ++a) {
                        for(b = -delta; b < delta; ++b) {
                            sum_position += (
                                kernel[a + delta][b + delta] * matrix[i + a][j + b]->val[k]);
                        }
                    }
                    convoluted->matrix[i][j]->updateColor(k, sum_position / kernelArea);
                    sum_position = 0;
                }
            }
        }
        matrix = convoluted->matrix;
    }

    void print() {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                if (matrix[i][j] == NULL) {
                    cout << "(invalid)";
                } else {
                    cout << *(matrix[i][j]);
                }
                cout << ' ';
            }
            cout << endl;
        }
    }
};

#endif // IMAGE_H
