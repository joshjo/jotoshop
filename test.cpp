#include <stdio.h>
#include "image.hpp"
#include "kernels.hpp"


using namespace std;


int main() {
    // cout << Pixel::R << endl;
    // char const * name = ;
    Image * img = new Image("pikachu.bmp");
    int kernelSize = 5;
    int ** kernel = blurKernel(kernelSize);
    img->equalize();
    img->convolution(kernelSize, kernel);
    printf("Hello world\n");
}

