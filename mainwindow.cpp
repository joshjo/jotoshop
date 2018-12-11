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
#include "image.hpp"
#include "kernels.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    img = new Image("/home/josue/Devel/jotoshop/jotoshop/tiger.bmp");
//    renderImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::renderImage(QGraphicsView *view)
{
    int width = img->width;
    int height = img->height;
    QImage image = QImage(width, height, QImage::Format_RGB32);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Pixel * pixel = img->matrix[i][j];
            image.setPixelColor(
                j, height - i - 1, qRgb(pixel->val[0], pixel->val[1], pixel->val[2]));
        }
    }

    QGraphicsScene * graphic = new QGraphicsScene(this);
    graphic->addPixmap(QPixmap::fromImage(image));
    view->setScene(graphic);
}

void MainWindow::renderImage()
{
    this->renderImage(ui->graphicsView);
}

void MainWindow::on_openFileButton_clicked()
{
    QString qFileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "",
            tr("Image (*.bmp);;"));
    QByteArray ba = qFileName.toLocal8Bit();
    char *fileName = ba.data();

    img = new Image(fileName);
    renderImage();

}

void MainWindow::on_pushButton_clicked()
{
    img->equalize();
    renderImage();
}

void MainWindow::on_thresholdButton_clicked()
{
    img->threshold();
    renderImage();
}

void MainWindow::on_convolutionButton_clicked()
{
    int kernelSize = 5;
    int ** kernel = blurKernel(kernelSize);

    img->convolution(kernelSize, kernel);
    renderImage();
}

void MainWindow::on_prewitConvolution_clicked()
{
    int ** kernel = prewitKernel();

    img->convolution(3, kernel);
    renderImage();
}

void MainWindow::on_interpolationButton_clicked()
{
    img = img->interpolation(2, 2);
//    img = newImage;
    renderImage();
}

void MainWindow::on_cmykButton_clicked()
{
    for (int c = 0; c < 4; c++) {
        QWidget *wdg = new QWidget;
        QLabel * label = new QLabel;
        QVBoxLayout* vlayout = new QVBoxLayout(wdg);
        QGraphicsView *view = new QGraphicsView();
        if (c == 0) {
            label->setText("Cyan");
        } else if (c == 1) {
            label->setText("Magenta");
        } else if (c == 2) {
            label->setText("Yellow");
        } else if (c == 3) {
            label->setText("Black");
        }

        vlayout->addWidget(label);
        vlayout->addWidget(view);

        int width = img->width;
        int height = img->height;
        QImage image = QImage(width, height, QImage::Format_Grayscale8);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel * pixel = img->matrix[i][j];
                int color = (int) (pixel->cmyk()[c] * 255);
                image.setPixelColor(
                    j, height - i - 1, qRgb(color, color, color));
            }
        }
        QGraphicsScene * graphic = new QGraphicsScene(this);
        graphic->addPixmap(QPixmap::fromImage(image));
        view->setScene(graphic);

        wdg->show();
    }
}

void MainWindow::on_hsvButton_clicked()
{
    for (int c = 0; c < 3; c++) {
        QWidget *wdg = new QWidget;
        QLabel * label = new QLabel;
        QVBoxLayout* vlayout = new QVBoxLayout(wdg);
        QGraphicsView *view = new QGraphicsView();
        if (c == 0) {
            label->setText("Hue");
        } else if (c == 1) {
            label->setText("Saturation");
        } else if (c == 2) {
            label->setText("Value");
        }

        vlayout->addWidget(label);
        vlayout->addWidget(view);

        int width = img->width;
        int height = img->height;
        QImage image = QImage(width, height, QImage::Format_Grayscale8);

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Pixel * pixel = img->matrix[i][j];
                int color = pixel->hsv()[c];
                image.setPixelColor(
                    j, height - i - 1, qRgb(color, color, color));
            }
        }
        QGraphicsScene * graphic = new QGraphicsScene(this);
        graphic->addPixmap(QPixmap::fromImage(image));
        view->setScene(graphic);

        wdg->show();
    }
}
