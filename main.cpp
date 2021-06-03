#include <iostream>
#include <QApplication>
#include <QFile>

#include "mainwindow.h"
#include "myrobot.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // +80 style points !
    QFile styleSheetFile("..\\Projet\\Diffnes.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}
