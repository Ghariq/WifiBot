#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "myrobot.h"

extern MyRobot christobal;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Aller let's go le WifiBot");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_forward_clicked()
{
    qDebug() << "avance christobal !";
    christobal.forward();
}


void MainWindow::on_backward_clicked()
{
    qDebug() << "recule christobal !";
    christobal.backward();
}
