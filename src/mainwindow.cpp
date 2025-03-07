#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene();
    clockDial = new ClockDial();
    scene->addItem(clockDial);

    ui->graphicsView->setScene(scene);
    connect(clockDial, &ClockDial::radiusUpdate, this, &MainWindow::updateDialRadius);
    connect(clockDial, &ClockDial::angleUpdated, this, &MainWindow::updateLineAngle);
    connect(clockDial, &ClockDial::MousePosUpdated, this, &MainWindow::updateMousePos);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDialRadius()
{
    ui->lineEdit_Radius->setText(QString::number(clockDial->getDialRadius()));

}

void MainWindow::updateLineAngle()
{
    ui->lineEdit_Angle->setText(QString::number(clockDial->getAngle()));

}

void MainWindow::updateMousePos()
{
    ui->lineEdit_PosX->setText(QString::number(clockDial->getMousePosition().x()));
    ui->lineEdit_PosY->setText(QString::number(clockDial->getMousePosition().y()));

}

