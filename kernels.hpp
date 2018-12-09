#ifndef KERNELS_H
#define KERNELS_H

int ** initKernel(int size, int def = 0) {
    int ** kernel = new int * [size];
    for (int i = 0; i < size; i++) {
        kernel[i] = new int [size];
        for (int j = 0; j < size; j++) {
            kernel[i][j] = def;
        }
    }
    return kernel;
}

int ** blurKernel(int size) {
    return initKernel(size, 1);
}

int ** prewitKernel() {
    int ** kernel = initKernel(3, 0);
    kernel[0][0] = -1;
    kernel[1][0] = -1;
    kernel[2][0] = -1;

    kernel[0][2] = 1;
    kernel[1][2] = 1;
    kernel[2][2] = 1;
    return kernel;
}

#endif // KERNELS_H
