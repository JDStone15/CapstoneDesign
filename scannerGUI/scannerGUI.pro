#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T20:04:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = scannerGUI
TEMPLATE = app

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lopencv_calib3d -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_video -lopencv_videoio -lopencv_videostab -lopencv_features2d -lopencv_flann -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts

SOURCES += main.cpp\
        mainwindow.cpp \
    startupwindow.cpp

HEADERS  += mainwindow.h \
    startupwindow.h

FORMS    += mainwindow.ui \
    startupwindow.ui
