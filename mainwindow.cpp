#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    //QMessageBox::information(this, "title", "Hello world");
    qDebug() << "yes";
}


void MainWindow::on_backward_clicked()
{
    //QMessageBox::information(this, "title", "Hello world");
    qDebug() << "yes";
}
