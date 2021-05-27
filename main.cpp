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

<<<<<<< HEAD
=======
    christobal.doConnect();

>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4
    return a.exec();
}
