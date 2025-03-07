#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <clockdial.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateDialRadius();
    void updateLineAngle();
    void updateMousePos();


private:
    Ui::MainWindow *ui;
    ClockDial *clockDial;
};
#endif // MAINWINDOW_H
