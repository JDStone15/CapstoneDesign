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
    QImage qimgOriginal;
    QTimer* tmrTimer;

public slots:
    void processFrameAndUpdateGui();


private slots:
  //  void on_pushButton_clicked();
    void on_on_btnPauseOrResume_clicked();
};

#endif // SCANWINDOW_H
