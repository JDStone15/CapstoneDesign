#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QDialog>

namespace Ui {
class StartupWindow;
}

class StartupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = 0);
    ~StartupWindow();

private:
    Ui::StartupWindow *ui;
};

#endif // STARTUPWINDOW_H
