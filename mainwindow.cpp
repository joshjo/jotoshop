#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <stdio.h>


unsigned char* readBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    return data;
}


unsigned char* ReadBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");

    if(f == NULL)
        throw "Argument Exception";

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

//    cout << endl;
//    cout << "  Name: " << filename << endl;
//    cout << " Width: " << width << endl;
//    cout << "Height: " << height << endl;

    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;

//            cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
    }

    fclose(f);
    return data;
}




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width    = 630;
    int height = 354;
    QImage image = QImage(width, height, QImage::Format_RGB32);

    char * f = "/home/josue/Devel/jotoshop/tiger.bmp";
    unsigned char* imageData = readBMP(f);
    int p = 0;
    int q = 100;

    for(int i = 1881; i < 1981; i++) {
        qDebug("%d - ", (int) imageData[i]);
    }
//    for(int x=0; x < q; x++) {
//        for(int y=0; y < q; y++) {
//            p = (x * width + y) * 3;
////            qDebug("-> %d", p);
//            int r = (int) imageData[p];
//            int g = (int) imageData[p + 1];
//            int b = (int) imageData[p + 2];
//            qDebug("color (%d, %d) %d - rgb(%d, %d, %d)\n", x, y, p, r, g, b);
//            image.setPixelColor(x, y, qRgb(r, g, b));
//        }
//    }

    qDebug("P size %d\n", width * height);

    QGraphicsScene * graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(graphic);

}

MainWindow::~MainWindow()
{
    delete ui;
}
