#ifndef SCANWINDOW_H
#define SCANWINDOW_H

#include <common.h>

#include <QDialog>

using namespace cv;

namespace Ui {
class ScanWindow;
}

class ScanWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScanWindow(QWidget *parent = 0);
    ~ScanWindow();

private:
    Ui::ScanWindow *ui;

    VideoCapture camera;
    Mat matOriginal;
    Mat HSVFeed;
    Mat threshold;
    QImage qimgOriginal;
    QTimer* tmrTimer;

    int H_MIN = 0;
    int S_MIN = 0;
    int V_MIN = 0;

    int H_MAX = 256;
    int S_MAX = 256;
    int V_MAX = 256;

public slots:
    void processFrameAndUpdateGui();


private slots:
  //  void on_pushButton_clicked();
    void on_on_btnPauseOrResume_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
};

#endif // SCANWINDOW_H
