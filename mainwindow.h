#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
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

private slots:
    void on_forward_pressed();
    void on_backward_pressed();
    void on_forward_left_pressed();
    void on_forward_right_pressed();
    void on_backward_left_pressed();
    void on_backward_right_pressed();
    void on_connexion_clicked();
    void on_disconnect_clicked();
    void on_middle_pressed();
    void display(bool connected);
    void displayCamera (bool display);
    void reloadDisplay(QByteArray retour);
    void changeBat(unsigned char bat);
    void reloadSpeed (int speed);

private:
    Ui::MainWindow *ui;
    MyRobot *christobal;
    QWebEngineView *view;
};
#endif // MAINWINDOW_H
