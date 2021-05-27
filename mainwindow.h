#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myrobot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void display(bool connected);

private slots:
    void on_forward_pressed();
    void on_backward_pressed();
    void on_left_pressed();
    void on_right_pressed();
    void on_connexion_clicked();
    void on_disconnect_clicked();
    void on_middle_pressed();

private:
    Ui::MainWindow *ui;
    MyRobot *christobal;
};
#endif // MAINWINDOW_H
