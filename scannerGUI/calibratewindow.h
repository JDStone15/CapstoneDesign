#ifndef CALIBRATEWINDOW_H
#define CALIBRATEWINDOW_H

#include <common.h>
#include <QDialog>

namespace Ui {
class CalibrateWindow;
}

class CalibrateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrateWindow(QWidget *parent = 0);
    ~CalibrateWindow();

    void drawLine(Mat img, Point start, Point end);

private:
    Ui::CalibrateWindow *ui;

    VideoCapture camera;
    Mat cameraFeed;
    QImage qimgCameraFeed;
    QTimer* tmrTimer;

public slots:
    void processFrameAndUpdateGui();


private slots:
    void on_okButton_clicked();
};

#endif // CALIBRATEWINDOW_H
