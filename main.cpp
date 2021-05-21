#include <iostream>
#include <QApplication>
#include "mainwindow.h"
#include "myrobot.h"

MyRobot christobal;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    christobal.doConnect();

    return a.exec();
}
