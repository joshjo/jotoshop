#include <stdio.h>
#include "image.hpp"

struct FileData {
    Pixel ** matrix;
    int width;
    int height;
};


Image * ReadBMP(char* filename)
{
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

    qDebug("width: %d", width);
    qDebug("height: %d", height);
    qDebug("dataOffset: %d", dataOffset);

    int row_padded = (width*3 + 3) & (~3);
    qDebug("row_padded : %d", row_padded);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;
    Pixel ** matrix;
    matrix = new Pixel * [height];
    fread(data, sizeof(unsigned char), dataOffset - HEADER_SIZE, f);
    Image * image = new Image(Pixel::B24, width, height);

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
//        matrix[i] = new Pixel[width];
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
//            matrix[i][j/3] = Pixel((int) data[j + 2], (int) data[j+1], (int) data[j]);
            image->setPixel(
                i, j / 3,
                (int) data[j + 2],
                (int) data[j + 1],
                (int) data[j],
            );
        }
    }

//    FileData * fd = new FileData();
//    fd->matrix = matrix;
//    fd->width = width;
//    fd->height = height;

    fclose(f);
    return fd;
}
