#include <QMessageBox>
#include <QWebEngineView>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myrobot.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Christobal");
    display(false);

    christobal = new MyRobot (this);
    view = new QWebEngineView (this);
    ui->camera->addWidget(view);
    view->load(QUrl("http://192.168.1.11:8080/?action=stream"));
    view->hide();

    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), ui->v_max_aff, SLOT(display(int)));
    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), this->christobal, SLOT(setMaxSpeed(int)));
    connect(this->christobal, SIGNAL(changeConnectState(bool)), this, SLOT(display(bool)));
    connect(this->christobal, SIGNAL(changeConnectState(bool)), this, SLOT(displayCamera(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_forward_pressed()
{
    christobal->forward();
    ui->vitesse_aff->display(christobal->getSpeed());
}

void MainWindow::on_backward_pressed()
{
    christobal->backward();
    ui->vitesse_aff->display(christobal->getSpeed());
}

void MainWindow::on_forward_left_pressed()
{
    christobal->left(true);
}

void MainWindow::on_forward_right_pressed()
{
    christobal->right(true);
}

void MainWindow::on_backward_left_pressed()
{
    christobal->left(false);
}

void MainWindow::on_backward_right_pressed()
{
    christobal->right(false);
}

void MainWindow::on_middle_pressed()
{
    christobal->beyblade();
}

void MainWindow::on_connexion_clicked()
{
    qDebug() << "A toi de jouer christobal !";
    christobal->doConnect();
}

void MainWindow::on_disconnect_clicked()
{
    qDebug() << "Revient christobal !";
    christobal->disConnect();
}

void MainWindow::display(bool connected)
{
    ui->forward->setEnabled(connected);
    ui->backward->setEnabled(connected);
    ui->forward_left->setEnabled(connected);
    ui->forward_right->setEnabled(connected);
    ui->backward_left->setEnabled(connected);
    ui->backward_right->setEnabled(connected);
    ui->left->setEnabled(connected);
    ui->right->setEnabled(connected);
    ui->middle->setEnabled(connected);
    ui->disconnect->setEnabled(connected);
    ui->v_max_aff->setEnabled(connected);
    ui->v_max_label->setEnabled(connected);
    ui->v_max_slider->setEnabled(connected);
    ui->vitesse_aff->setEnabled(connected);
    ui->vitesse_label->setEnabled(connected);

    ui->connexion_ok->setChecked(connected);

    ui->connexion->setEnabled(!connected);
}

void MainWindow::displayCamera(bool display)
{
    if (display)
    {
        view->show();
    } else view->hide();
}
