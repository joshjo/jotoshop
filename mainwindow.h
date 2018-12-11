#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "image.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();

    void on_pushButton_clicked();

    void on_thresholdButton_clicked();

    void on_convolutionButton_clicked();

    void on_prewitConvolution_clicked();

    void on_interpolationButton_clicked();

    void on_cmykButton_clicked();

    void on_hsvButton_clicked();

private:
    Ui::MainWindow *ui;
    Image * img;

    void renderImage(QGraphicsView *view);
    void renderImage();
};

#endif // MAINWINDOW_H
