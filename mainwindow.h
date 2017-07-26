#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void equationChanged(std::string);
    void colorWireframeChanged(QColor color);
    void colorBackgroundChanged(QColor color);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnPlot_clicked();
    void on_btnWireframeColor_clicked();
    void on_btnBackgroundColor_clicked();
    void setNameOfAnimateBtn(std::string name);

private:
    Ui::MainWindow *ui;
    QColorDialog *m_colorDialogWireframe, *m_colorDialogBackground;
};

#endif // MAINWINDOW_H
