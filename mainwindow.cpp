#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Here I am using lambda-slot
    m_colorDialogWireframe = new QColorDialog(Qt::green, this);
    connect(m_colorDialogWireframe, &QDialog::accepted, [=](){
        emit colorWireframeChanged(m_colorDialogWireframe->currentColor());
    });
    connect(this, SIGNAL(colorWireframeChanged(QColor)), ui->canvas, SLOT(setWireframeColor(QColor)));

    m_colorDialogBackground = new QColorDialog(Qt::black, this);
    connect(m_colorDialogBackground, &QDialog::accepted, [=](){
        emit colorBackgroundChanged(m_colorDialogBackground->currentColor());
    });
    connect(this, SIGNAL(colorBackgroundChanged(QColor)), ui->canvas, SLOT(setBackgroundColor(QColor)));

    connect(this, SIGNAL(equationChanged(std::string)), ui->canvas, SLOT(setExpression(std::string)));

    //Rotation

    connect(ui->btnRotateXLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateXLeft()));
    connect(ui->btnRotateYLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateYLeft()));
    connect(ui->btnRotateZLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateZLeft()));
    connect(ui->btnRotateXRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateXRight()));
    connect(ui->btnRotateYRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateYRight()));
    connect(ui->btnRotateZRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(rotateZRight()));

    //Translation

    connect(ui->btnTranslateXLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateXLeft()));
    connect(ui->btnTranslateYLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateYLeft()));
    connect(ui->btnTranslateZLeft, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateZLeft()));
    connect(ui->btnTranslateXRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateXRight()));
    connect(ui->btnTranslateYRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateYRight()));
    connect(ui->btnTranslateZRight, SIGNAL(clicked(bool)), ui->canvas, SLOT(translateZRight()));

    //Zoomming

    connect(ui->btnZoomIn, SIGNAL(clicked(bool)), ui->canvas, SLOT(zoomIn()));
    connect(ui->btnZoomOut, SIGNAL(clicked(bool)), ui->canvas, SLOT(zoomOut()));

    //Gris settings

    connect(ui->spinnerXmin, SIGNAL(valueChanged(double)), ui->canvas, SLOT(setXmin(double)));
    connect(ui->spinnerXmax, SIGNAL(valueChanged(double)), ui->canvas, SLOT(setXmax(double)));
    connect(ui->spinner_dX, SIGNAL(valueChanged(double)), ui->canvas, SLOT(set_dX(double)));
    connect(ui->spinnerYmin, SIGNAL(valueChanged(double)), ui->canvas, SLOT(setYmin(double)));
    connect(ui->spinnerYmax, SIGNAL(valueChanged(double)), ui->canvas, SLOT(setYmax(double)));
    connect(ui->spinner_dY, SIGNAL(valueChanged(double)), ui->canvas, SLOT(set_dY(double)));

    //Animation

    connect(ui->btnAnimation, SIGNAL(clicked(bool)), ui->canvas, SLOT(animation()));
    connect(ui->canvas, SIGNAL(animationChanged(std::string)), this, SLOT(setNameOfAnimateBtn(std::string)));

    //Settings

    connect(ui->btnSettings, SIGNAL(clicked(bool)), ui->canvas, SLOT(loadSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnPlot_clicked()
{
    emit equationChanged(ui->txtEquation->text().toStdString());
}

void MainWindow::on_btnWireframeColor_clicked()
{
    m_colorDialogWireframe->show();
}

void MainWindow::on_btnBackgroundColor_clicked()
{
    m_colorDialogBackground->show();
}

void MainWindow::setNameOfAnimateBtn(std::string name)
{
    ui->btnAnimation->setText(QString::fromStdString(name));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key == Qt::Key_Return)
    {
        //qDebug() << "Enter";
        emit equationChanged(ui->txtEquation->text().toStdString());
    }
    else if(key == Qt::Key_Q || key == Qt::Key_Escape)
    {
        close();
    }
}
