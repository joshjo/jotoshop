#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QMessageBox>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>

struct Pixel {
    int r;
    int g;
    int b;

    Pixel(int r,int g,int b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Pixel() {
        r = g = b = 0;
    }
};

struct FileData {
    Pixel ** matrix;
    int width;
    int height;
};


FileData * ReadBMP(char* filename)
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

    for(int i = 0; i < height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        matrix[i] = new Pixel[width];
        for(int j = 0; j < width*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            matrix[i][j/3] = Pixel((int) data[j + 2], (int) data[j+1], (int) data[j]);
        }
    }

    FileData * fd = new FileData();
    fd->matrix = matrix;
    fd->width = width;
    fd->height = height;

    fclose(f);
    return fd;
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_applyButton_clicked()
{
    QString qFileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "",
            tr("Image (*.bmp);;"));
//    qDebug(qUtf8Printable(fileName));
    QByteArray ba = qFileName.toLocal8Bit();
    char *fileName = ba.data();

    FileData * fd = ReadBMP(fileName);
    int width = fd->width;
    int height = fd->height;
    QImage image = QImage(width, height, QImage::Format_RGB32);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel pixel = fd->matrix[i][j];
            image.setPixelColor(j, height - i - 1, qRgb(pixel.r, pixel.g, pixel.b));
        }
    }

    QGraphicsScene * graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(graphic);
}

void MainWindow::on_pushButton_clicked()
{

}
