#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>
#include <parser.h>

//Сделать ифедев дебаг и дисплейный список

struct Point
{
    double x, y, z;
};

typedef std::vector<std::vector<Point>> WireframeGrid;

class MyGLWidget : public QGLWidget
{
    Q_OBJECT

public:

    explicit MyGLWidget(QWidget *parent = nullptr);

signals:

    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void animationChanged(std::string btnTitle);

public slots:

    void setExpression(std::string m_expr);

    void setXmin(double val);
    void setXmax(double val);
    void set_dX(double val);
    void setYmin(double val);
    void setYmax(double val);
    void set_dY(double val);

    // slots for xyz-rotation

    void rotateXLeft();
    void rotateYLeft();
    void rotateZLeft();
    void rotateXRight();
    void rotateYRight();
    void rotateZRight();

    // slots for xyz-translation

    void translateXLeft();
    void translateYLeft();
    void translateZLeft();
    void translateXRight();
    void translateYRight();
    void translateZRight();

    // slots for zooming

    void zoomIn();
    void zoomOut();

    //slots for colors

    void setWireframeColor(QColor color);
    void setBackgroundColor(QColor color);

    //Animation

    void animation();

    //Load frustum settings

    void loadSettings();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    std::string m_expr;
    double m_Xmin, m_Xmax, m_dX, m_Ymin, m_Ymax, m_dY, m_Zmin, m_Zmax;
    std::pair<bool, double> m_rotateX;
    std::pair<bool, double> m_rotateY;
    std::pair<bool, double> m_rotateZ;
    QColor m_colorWireframe, m_colorBackground;
    bool m_zoomIn = false;
    bool m_zoomOut = false;
    bool m_rotateXLeft = false;
    bool m_rotateXRight = false;
    bool m_rotateYLeft = false;
    bool m_rotateYRight = false;
    bool m_rotateZLeft = false;
    bool m_rotateZRight = false;
    bool m_translateXLeft = false;
    bool m_translateXRight = false;
    bool m_translateYLeft = false;
    bool m_translateYRight = false;
    bool m_translateZLeft = false;
    bool m_translateZRight = false;
    bool m_changeBackgroundColor= false;
    QMessageBox *m_box;
    static std::string PATH_TO_SETTINGS_FILE;
    double m_frustumLeft, m_frustumRight, m_frustumBottom, m_frustumTop,
           m_frustumNearVal, m_frustumFarVal;
    double m_initTransX, m_initTransY, m_initTransZ;
    double m_initRotX, m_initRotY, m_initRotZ;
    double m_transStepX, m_transStepY, m_transStepZ;
    double m_rotStepX, m_rotStepY, m_rotStepZ;
    double m_scaleFactor, m_timerDelay;
    QTimer m_timer;
    WireframeGrid m_grid_l;
    WireframeGrid m_grid_t;
    //GLint m_nSurface; //Display list number

    QPoint m_lastPos;

    void calculatePoints();
    void printSomePoints();
    void calculateZlimits();
    void draw();
    //GLint createSurface();
};

#endif // MYGLWIDGET_H
