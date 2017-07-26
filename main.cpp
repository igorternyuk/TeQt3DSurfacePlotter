#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <GL/gl.h>
#include <GL/glut.h>

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    QRect screen = QApplication::desktop()->geometry();
    int dx = (screen.width() - w.width()) / 2;
    int dy = (screen.height() - w.height()) / 2;
    w.move(dx, dy);
    w.show();

    return a.exec();
}
