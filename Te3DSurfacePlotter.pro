#-------------------------------------------------
#
# Project created by QtCreator 2017-07-26T12:52:18
#
#-------------------------------------------------

QT       += core gui  opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Te3DSurfacePlotter
TEMPLATE = app

DEFINES -= DEBUG

SOURCES += main.cpp \
    mainwindow.cpp \
    myglwidget.cpp \
    parser.cpp

HEADERS  += \
    mainwindow.h \
    myglwidget.h \
    parser.h

FORMS    += \
    mainwindow.ui

LIBS += -lglut -lGLU -lGL -lGLEW

QMAKE_CXXFLAGS += -std=c++14
