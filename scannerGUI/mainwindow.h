#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <secondwindow.h>


#define LOGO_IMAGE_IMAC "/Users/jacobstone/Documents/Github/CapstoneDesign/scannerGUI/logo.gif"
#define LOGO_IMAGE_MACBOOK "/Users/jacobstone/Documents/CapstoneDesign/scannerGUI/logo.gif"

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
