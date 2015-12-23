#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T20:24:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = CalcHistCalculator
TEMPLATE = app


SOURCES += main.cpp\
        calchistcalculator.cpp

HEADERS  += calchistcalculator.h

FORMS    += calchistcalculator.ui

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_calib3d

