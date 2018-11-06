#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>


unsigned char* readBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f);

    printf("nice reader\n");
    int width = *(int*) & info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size];
    fread(data, sizeof(unsigned char), size, f);
    fclose(f);

//    printf("width %d\n", width);
//    printf("height %d\n", height);

    for(i = 0; i < size; i += 3)
    {
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
//        printf("%zu", (size_t)tmp);
    }
    for (int i = 0; i < size; i++) {
//        printf("%zu|", (size_t) data[i]);
    }
    return data;
}





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width = 630;
    int height = 354;
    QImage image = QImage(width, height, QImage::Format_RGB32);

    char * f = "/home/josue/Devel/MCS/images/BMP/tiger.bmp";
    unsigned char* imageData = readBMP(f);

    for(int l=0; l < width; l++) {
        for(int c=0; c < height; c++) {
            int p = (l * width + c) * 3;
            int r = (int) imageData[p];
            int g = (int) imageData[p + 1];
            int b = (int) imageData[p + 2];
            image.setPixel(l, c, qRgb(r,g, b));
        }
    }

    QGraphicsScene * graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(graphic);

}

MainWindow::~MainWindow()
{
    delete ui;
}
