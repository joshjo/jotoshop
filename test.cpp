#include <stdio.h>
#include "image.hpp"
#include "kernels.hpp"


using namespace std;


int main() {
    // Image * img = new Image(Pixel::B24, 3, 2);
    // Image * img = new Image("pikachu.bmp");
    // img->setPixel(0, 1, 0, 255, 255);
    // img->setPixel(0, 1, 127, 127, 127);
    // img->setPixel(0, 2, 255, 255, 255);
    // img->setPixel(1, 1, 127, 127, 127);
    // img->setPixel(1, 2, 255, 255, 255);
    // img->print();
    // img = img->interpolation(2, 2);
    // cout << " --- " << endl;
    // img->print();

    // Pixel a(Pixel::B24);
    // Pixel b(Pixel::B24);
    // a.val[0] = 120;
    // a.val[1] = 110;
    // a.val[2] = 100;

    // pint * hsv = a.hsv();
    // for (int i = 0; i < 3; i++) {
    //     cout << hsv[i] << endl;
    // }


    // cout << "cyan: " << a.cyan() << endl;
    // cout << "mage: " << a.magenta() << endl;
    // cout << "yell: " << a.yellow() << endl;
    // cout << "blakc: " << a.black() << endl;

    // b.val[0] = 100;
    // b.val[1] = 110;
    // b.val[2] = 120;

    // Pixel c = 0.5 * (a - b);
    // cout << c << endl;
    // // Pixel inter = pixel.interpolate(copy);
    // // cout << inter << endl;
    // // cout << Pixel::R << endl;
    // // char const * name = ;
    Image * img = new Image("tiger.bmp");
    // img = img->interpolation(2, 2);
    // img->print();
    int kernelSize = 5;
    int ** kernel = blurKernel(kernelSize);
    img->equalize();
    img->convolution(kernelSize, kernel);
    printf("Hello world\n");
}


