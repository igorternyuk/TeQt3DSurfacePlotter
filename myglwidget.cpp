#include "myglwidget.h"
#include <sstream>
#include <fstream>
#include <GL/glut.h>

#ifdef DEBUG
#include <QDebug>
#endif

using namespace iat;

MyGLWidget::MyGLWidget(QWidget *parent):
    QGLWidget(parent), m_expr("0"),
    m_Xmin(-500), m_Xmax(+500), m_dX(50), m_Ymin(-500), m_Ymax(+500), m_dY(50),
    m_Zmin(-500), m_Zmax(+500),
    m_rotateX(false, 0.0), m_rotateY(false, 0.0),m_rotateZ(false, 0.0),
    m_colorWireframe(Qt::green), m_colorBackground(Qt::black)
{
    m_box = new QMessageBox(this);
    loadSettings();
    calculatePoints();
    calculateZlimits();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
}

std::string MyGLWidget::PATH_TO_SETTINGS_FILE = "config.dat";

void MyGLWidget::setExpression(std::string expr)
{
    m_expr = expr;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::setXmin(double val)
{
    m_Xmin = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::setXmax(double val)
{
    m_Xmax = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::set_dX(double val)
{
    m_dX = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::setYmin(double val)
{
    m_Ymin = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::setYmax(double val)
{
    m_Ymax = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::set_dY(double val)
{
    m_dY = val;
    calculatePoints();
    calculateZlimits();
    updateGL();
}

void MyGLWidget::rotateXLeft()
{
    m_rotateXLeft = true;
    updateGL();
}

void MyGLWidget::rotateYLeft()
{
    m_rotateYLeft = true;
    updateGL();
}

void MyGLWidget::rotateZLeft()
{
    m_rotateZLeft = true;
    updateGL();
}

void MyGLWidget::rotateXRight()
{
    m_rotateXRight = true;
    updateGL();
}

void MyGLWidget::rotateYRight()
{
    m_rotateYRight = true;
    updateGL();
}

void MyGLWidget::rotateZRight()
{
    m_rotateZRight = true;
    updateGL();
}

// slots for xyz-translation

void MyGLWidget::translateXLeft()
{
    m_translateXLeft = true;
    updateGL();
}

void MyGLWidget::translateYLeft()
{
    m_translateYLeft = true;
    updateGL();
}

void MyGLWidget::translateZLeft()
{
    m_translateZLeft = true;
    updateGL();
}

void MyGLWidget::translateXRight()
{
    m_translateXRight = true;
    updateGL();
}

void MyGLWidget::translateYRight()
{
    m_translateYRight = true;
    updateGL();
}

void MyGLWidget::translateZRight()
{
    m_translateZRight = true;
    updateGL();
}

void MyGLWidget::zoomIn()
{
    m_zoomIn = true;
    updateGL();
}

void MyGLWidget::zoomOut()
{
    m_zoomOut = true;
    updateGL();
}

void MyGLWidget::setWireframeColor(QColor color)
{
    m_colorWireframe = color;
    updateGL();
}

void MyGLWidget::setBackgroundColor(QColor color)
{
    m_colorBackground = color;
    m_changeBackgroundColor = true;
    updateGL();
}

void MyGLWidget::animation()
{
    if(m_timer.isActive())
    {
        m_timer.stop();
        emit animationChanged("Start animation");
    }
    else
    {
        m_timer.start(m_timerDelay);
        emit animationChanged("Stop animation");
    }
}

void MyGLWidget::loadSettings()
{
/*
[Frustum left right bottom top near_val far_val]
-500 500 -500 500 500 2000
[Initial translation]
0 0 -1200
[Initial rotation]
-30 0 0
[Translation steps(x,y,z)]
10 10 10
[Rotation steps(x,y,z)]
5 5 5
[Scale factor]
1.1
[Animation timer delay(ms)]
1
*/
    enum LoadState{FRUSTUM, INIT_ROT, INIT_TRANS, TRANS_STEPS, ROT_STEPS,
                   SCALE_FACTOR, TIMER_DELAY, STOP};

    std::ifstream file(PATH_TO_SETTINGS_FILE);
    if(file.is_open())
    {
        LoadState ls = STOP;
        std::string line;
        std::stringstream ss;
        while(!file.eof())
        {
            std::getline(file, line);
            if(line == "[Frustum left right bottom top near_val far_val]")
            {
                ls = FRUSTUM;
            }
            else if(line == "[Initial translation]")
            {
                ls = INIT_TRANS;
            }
            else if(line == "[Initial rotation]")
            {
                ls = INIT_ROT;
            }
            else if(line == "[Translation steps(x,y,z)]")
            {
                ls = TRANS_STEPS;
            }
            else if(line == "[Rotation steps(x,y,z)]")
            {
                ls = ROT_STEPS;
            }
            else if(line == "[Scale factor]")
            {
                ls = SCALE_FACTOR;
            }
            else if(line == "[Animation timer delay(ms)]")
            {
                ls = TIMER_DELAY;
            }
            else
            {
                ls = STOP;
            }

            std::getline(file, line);

            switch(ls)
            {
                case FRUSTUM:
                    ss.clear();
                    ss << line;
                    ss >> m_frustumLeft >> m_frustumRight >> m_frustumBottom >>
                            m_frustumTop >> m_frustumNearVal >> m_frustumFarVal;
                    break;
                case INIT_TRANS:
                    ss.clear();
                    ss << line;
                    ss >> m_initTransX >> m_initTransY >> m_initTransZ;
                    break;
                case INIT_ROT:
                    ss.clear();
                    ss << line;
                    ss >> m_initRotX >> m_initRotY >> m_initRotZ;
                    break;
                case TRANS_STEPS:
                    ss.clear();
                    ss << line;
                    ss >> m_transStepX >> m_transStepY >> m_transStepZ;
                    break;
                case ROT_STEPS:
                    ss.clear();
                    ss << line;
                    ss >> m_rotStepX >> m_rotStepY >> m_rotStepZ;
                    break;
                case SCALE_FACTOR:
                    ss.clear();
                    ss << line;
                    ss >> m_scaleFactor;
                    break;
                case TIMER_DELAY:
                    ss.clear();
                    ss << line;
                    ss >> m_timerDelay;
                    break;
                default:
                    break;
            }
            if(ls == STOP) break;
        }
        file.close();
    }
    else
    {
        QMessageBox box;
        box.setText("Could not load settigs file. Default settings will be used.");
        box.exec();
        m_frustumLeft = -500;
        m_frustumRight = +500;
        m_frustumBottom = -500;
        m_frustumTop = +500;
        m_frustumNearVal = +500;
        m_frustumFarVal = 2000;
        m_initTransX = 0;
        m_initTransY = 0;
        m_initTransZ = -1200;
        m_initRotX = -30;
        m_initRotY = 0;
        m_initRotZ = 0;
        m_transStepX = 10;
        m_transStepY = 10;
        m_transStepZ = 10;
        m_rotStepX = 5;
        m_rotStepY = 5;
        m_rotStepZ = 5;
        m_scaleFactor = 1.1;
        m_timerDelay = 16;
    }
}

void MyGLWidget::initializeGL()
{
    qglClearColor(m_colorBackground);
    qglColor(Qt::black);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(m_frustumLeft, m_frustumRight, m_frustumBottom,
              m_frustumTop, m_frustumNearVal, m_frustumFarVal);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(m_initTransX, m_initTransY, m_initTransZ);
    glRotatef(m_initRotX, 1, 0, 0);
    glRotatef(m_initRotY, 0, 1, 0);
    glRotatef(m_initRotZ, 0, 0, 1);
    glViewport(0, 0, this->width(), this->height());
}

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(m_timer.isActive())
    {
        glRotatef(0.5, 1, 1, 1); //Animation
    }
    else
    {
        if(m_rotateX.first)
        {
            glRotatef(m_rotateX.second, 1, 0, 0);
            m_rotateX.first = false;
        }
        if(m_rotateY.first)
        {
            glRotatef(m_rotateY.second, 0, 1, 0);
            m_rotateY.first = false;
        }
        if(m_rotateZ.first)
        {
            glRotatef(m_rotateZ.second, 0, 0, 1);
            m_rotateZ.first = false;
        }

        if(m_rotateXLeft)
        {
           glRotatef(-m_rotStepX, 1.0, 0.0, 0.0);
           m_rotateXLeft = false;
        }
        if(m_rotateXRight)
        {
            glRotatef(+m_rotStepX, 1.0, 0.0, 0.0);
            m_rotateXRight = false;
        }

        if(m_rotateYLeft)
        {
           glRotatef(-m_rotStepY, 0.0, 1.0, 0.0);
           m_rotateYLeft = false;
        }
        if(m_rotateYRight)
        {
            glRotatef(+m_rotStepY, 0.0, 1.0, 0.0);
            m_rotateYRight = false;
        }

        if(m_rotateZLeft)
        {
           glRotatef(+m_rotStepZ, 0.0, 0.0, 1.0);
           m_rotateZLeft = false;
        }
        if(m_rotateZRight)
        {
            glRotatef(-m_rotStepZ, 0.0, 0.0, 1.0);
            m_rotateZRight = false;
        }

        if(m_translateXLeft)
        {
            glTranslatef(-m_transStepX, 0, 0);
            m_translateXLeft = false;
        }
        if(m_translateXRight)
        {
            glTranslatef(+m_transStepX, 0, 0);
            m_translateXRight = false;
        }

        if(m_translateYLeft)
        {
            glTranslatef(0, -m_transStepY, 0);
            m_translateYLeft = false;
        }
        if(m_translateYRight)
        {
            glTranslatef(0, +m_transStepY, 0);
            m_translateYRight = false;
        }

        if(m_translateZLeft)
        {
            glTranslatef(0, 0, -m_transStepZ);
            m_translateZLeft = false;
        }
        if(m_translateZRight)
        {
            glTranslatef(0, 0, +m_transStepZ);
            m_translateZRight = false;
        }

        if(m_zoomIn)
        {
            glScalef(m_scaleFactor, m_scaleFactor, m_scaleFactor);
            m_zoomIn = false;
        }
        if(m_zoomOut)
        {
            glScalef(1 / m_scaleFactor, 1 / m_scaleFactor, 1 / m_scaleFactor);
            m_zoomOut = false;
        }
    }

    if(m_changeBackgroundColor)
    {
        qglClearColor(m_colorBackground);
        m_changeBackgroundColor = false;
    }

    //glCallList(m_nSurface);
    draw();
}

void MyGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    /*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0,0,5,0,0,0,0,1,0);*/
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if(event->buttons() & Qt::LeftButton)
    {
        m_rotateX.first = true;
        m_rotateX.second = dy;
        m_rotateY.first = true;
        m_rotateY.second = dx;
    }
    else if(event->buttons() & Qt::RightButton)
    {
        m_rotateX.first = true;
        m_rotateX.second = dy;
        m_rotateZ.first= true;
        m_rotateZ.second = dx;
    }
    m_lastPos = event->pos();
    updateGL();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    if(delta > 0)
    {
        m_zoomIn = true;
    } else if(delta < 0)
    {
        m_zoomOut = true;
    }
    updateGL();
}

void MyGLWidget::calculatePoints()
{
    try
    {
        m_grid_l.clear();
        m_grid_t.clear();
        std::vector<std::pair<char, double>> vars;
        for(double x = m_Xmin; x < m_Xmax; x += m_dX)
        {
            std::vector<Point> tmp;
            for(double y = m_Ymin; y < m_Ymax; y += m_dY)
            {
                vars.clear();
                vars.emplace_back('X', x);
                vars.emplace_back('Y', y);
                Parser parser(m_expr, vars);
                Point p{x,y,parser.calculateExpression()};
                tmp.push_back(p);
            }
            m_grid_l.push_back(tmp);
        }
        //Horizontal lines

        for(double y = m_Ymin; y < m_Ymax; y += m_dY)
        {
            std::vector<Point> tmp;
            for(double x = m_Xmin; x < m_Xmax; x += m_dX)
            {
                vars.clear();
                vars.emplace_back('X', x);
                vars.emplace_back('Y', y);
                Parser parser(m_expr, vars);
                Point p{x,y,parser.calculateExpression()};
                tmp.push_back(p);
            }
            m_grid_t.push_back(tmp);
        }

    }
    catch(std::exception &ex)
    {
        //qDebug() << "Parser exception";
        m_box->setText(QString::fromStdString(ex.what()));
        m_box->show();
    }
}

void MyGLWidget::draw()
{
    //Vertical lines
    qglColor(m_colorWireframe);

    //Vertical lines

    for(const auto &line: m_grid_l)
    {
        glBegin(GL_LINE_STRIP);
        for(const auto &p: line)
        {
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
    }
    //Horizontal lines

    for(const auto &line: m_grid_t)
    {
        glBegin(GL_LINE_STRIP);
        for(const auto &p: line)
        {
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
    }

    glBegin(GL_LINES);

    //X-axis

    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(m_Xmin * 1.1, 0, 0);
    glVertex3d(m_Xmax * 1.1, 0, 0);

    //Y-axis

    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0, m_Ymin * 1.1, 0);
    glVertex3d(0, m_Ymax * 1.1, 0);

    //Z-axis

    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0, 0, m_Zmin * 1.1);
    glVertex3d(0, 0, m_Zmax * 1.1);
    glEnd();

    //Draw axis labels

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos3d(m_Xmax * 1.1 * 1.05, 0, 10);
    char ch = 'X';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos3d(0, m_Ymax * 1.1 * 1.05, 0);
    ch = 'Y';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    glColor3f(0.0f, 0.0f, 1.0f);
    glRasterPos3d(0, 0, m_Zmax * 1.1 * 1.05);
    ch = 'Z';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
}

/*GLint MyGLWidget::createSurface()
{
    //GLint n = glGenLists(1);
    //glNewList(n, GL_COMPILE);

    //Vertical lines
    qglColor(m_colorWireframe);

    //Vertical lines

    for(const auto &line: m_grid_l)
    {
        glBegin(GL_LINE_STRIP);
        for(const auto &p: line)
        {
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
    }
    //Horizontal lines

    for(const auto &line: m_grid_t)
    {
        glBegin(GL_LINE_STRIP);
        for(const auto &p: line)
        {
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
    }

    glBegin(GL_LINES);

    //X-axis

    glColor3d(1.0, 0.0, 0.0);
    glVertex3d(m_Xmin * 1.1, 0, 0);
    glVertex3d(m_Xmax * 1.1, 0, 0);

    //Y-axis

    glColor3d(0.0, 1.0, 0.0);
    glVertex3d(0, m_Ymin * 1.1, 0);
    glVertex3d(0, m_Ymax * 1.1, 0);

    //Z-axis

    glColor3d(0.0, 0.0, 1.0);
    glVertex3d(0, 0, m_Xmin * 0.75);
    glVertex3d(0, 0, m_Xmax * 0.75);
    glEnd();

    //Draw axis labels

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos3d(m_Xmax * 1.1 * 1.05, 0, 10);
    char ch = 'X';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    glColor3f(0.0f, 1.0f, 0.0f);
    glRasterPos3d(0, m_Ymax * 1.1 * 1.05, 0);
    ch = 'Y';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    glColor3f(0.0f, 0.0f, 1.0f);
    glRasterPos3d(0, 0, m_Xmax * 0.75 * 1.05);
    ch = 'Z';
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);

    //glEndList();
    //return n;
}*/

//This function is only for debugging

void MyGLWidget::printSomePoints()
{
    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
        {
            //qDebug() << "x = " << m_grid_l[i][j].x << "y = " << m_grid_l[i][j].y << " z = " << m_grid_l[i][j].z ;
        }
    }

    for(int i = 0; i < 10; ++i)
    {
        for(int j = 0; j < 5; ++j)
        {
           // qDebug() << "x = " << m_grid_t[i][j].x << "y = " << m_grid_t[i][j].y << " z = " << m_grid_t[i][j].z ;
        }
    }
}

void MyGLWidget::calculateZlimits()
{
    //m_Zmax, m_Zmin
    for(auto line: m_grid_l)
    {
        for(auto point : line)
        {
            if(point.z > m_Zmax)
            {
                m_Zmax = point.z;
            }
            if(point.z < m_Zmin)
            {
                m_Zmin = point.z;
            }
        }
    }
    for(auto line: m_grid_t)
    {
        for(auto point : line)
        {
            if(point.z > m_Zmax)
            {
                m_Zmax = point.z;
            }
            if(point.z < m_Zmin)
            {
                m_Zmin = point.z;
            }
        }
    }
}
