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

    void send();
    void goForward();
    void goForwardLeft();
    void goForwardRight();
    void goBackward();
    void goBackwardLeft();
    void goBackwardRight();
    void goBeybladeLeft();
    void goBeybladeRight();

    void changeMode();

    void cameraUp();
    void cameraDown();
    void cameraLeft();
    void cameraRight();


    void changeBat(unsigned char bat);
    void changeCapteur(unsigned char avant_gauche, unsigned char avant_droit, unsigned char arriere_gauche, unsigned char arriere_droit);

private slots:
    void display(bool connected);
    void displayCamera (bool display);
    void reloadDisplay(QByteArray retour);

    void on_forward_pressed();
    void on_backward_pressed();
    void on_forward_left_pressed();
    void on_forward_right_pressed();
    void on_backward_left_pressed();
    void on_backward_right_pressed();
    void on_left_pressed();
    void on_right_pressed();

    void on_connexion_clicked();
    void on_disconnect_clicked();
    void on_middle_pressed();

private:
    Ui::MainWindow *ui;
    MyRobot *christobal;
    QWebEngineView *view;
    QWebEngineView *url;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    bool _forward;
    bool _backward;
    bool _left;
    bool _right;
    bool _beyblade;
    bool _connected;
    bool _robot_mode;
};
#endif // MAINWINDOW_H
