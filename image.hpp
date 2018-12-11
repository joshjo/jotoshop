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

        int i, j;
# pragma omp parallel for num_threads(4) collapse(2) private(i, j)
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
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

        int i, j;

        for (i = 0; i < mode; i++) {
            Histogram hist = currentHistogram[i];
            float sum = 0;

            for (j = 0; j < maxColor; j++) {
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
        int i, j;
# pragma omp parallel for num_threads(4) collapse(2) private(i, j)
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                pint color = 0;
                int avg = matrix[i][j]->averageColor();
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
        int i, j;

# pragma omp parallel for num_threads(4) collapse(2) private(i, j)
        for (i = 0; i < this->height; i++) {
            for (j = 0; j < this->width; j++) {
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

# pragma omp parallel for num_threads(4) collapse(2) private(sum_position, i, j, a, b)
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

    Image * interpolation(int zoomX, int zoomY) {
        int newWidth = width * zoomX;
        int newHeight = height * zoomY;
        Image * newImage = new Image(mode, newWidth, newHeight);
        int i, j;

# pragma omp parallel for num_threads(4) collapse(2) private(i, j)
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                newImage->matrix[i * zoomY][j * zoomX] = new Pixel(*(matrix[i][j]));
            }
        }

        for(int j = 0; j < height; j++){
            for (int i = 0; i + 1 < width; i++) {
                int x1 = i * zoomX;
                int x2 = (i + 1) * zoomX;
                Pixel f_x1(*(matrix[j][i]));
                Pixel f_x2(*(matrix[j][i + 1]));
                for (int a = 1; a < zoomX; a++) {
                    int x = (i * zoomX) + a;
                    float factor = float(x - x1) / (x2 - x1);
                    Pixel newPixel = f_x1 + (factor * (f_x2 - f_x1));
                    newImage->matrix[j * zoomY][i * zoomX + a] = new Pixel(newPixel);
                }
                //
            }
        }

        for (int i = 0; i < newWidth; i++) {
            int y1, y2;
            for (int y = 0; (y + 1) < newHeight; y++) {
                if ((y % zoomX) == 0) {
                    y2 = y;
                    y1 = y + zoomX;
                    continue;
                }
                Pixel r1(*(newImage->matrix[y1][i]));
                Pixel r2(*newImage->matrix[y2][i]);
                float factor = float(y - y2) / (y2 - y1);
                Pixel px = r2 + (factor * (r1 - r2));
                newImage->matrix[y][i] = new Pixel(px);
            }
        }

        return newImage;
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
