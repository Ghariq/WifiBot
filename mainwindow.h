#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< HEAD
#include "myrobot.h"
=======
>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
<<<<<<< HEAD
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
=======

private slots:
    void on_forward_clicked();

    void on_backward_clicked();

private:
    Ui::MainWindow *ui;
>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4

};
#endif // MAINWINDOW_H
