#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "myrobot.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Christobal");
    display(false);
    christobal = new MyRobot (this);
    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), ui->v_max_aff, SLOT(display(int)));
    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), this->christobal, SLOT(setMaxSpeed(int)));
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

void MainWindow::on_left_pressed()
{
    christobal->left();
}

void MainWindow::on_right_pressed()
{
    christobal->right();
}

void MainWindow::on_connexion_clicked()
{
    qDebug() << "A toi de jouer christobal !";
    christobal->doConnect();

    bool is_connected = christobal->isConnected();
    if (is_connected)
    {
        display(true);
    }
}

void MainWindow::on_disconnect_clicked()
{
    qDebug() << "Revient christobal !";
    christobal->disConnect();
    display(false);
}

void MainWindow::display(bool connected)
{
    ui->forward->setEnabled(connected);
    ui->backward->setEnabled(connected);
    ui->left->setEnabled(connected);
    ui->right->setEnabled(connected);
    ui->middle->setEnabled(connected);
    ui->disconnect->setEnabled(connected);
    ui->v_max_aff->setEnabled(connected);
    ui->v_max_label->setEnabled(connected);
    ui->v_max_slider->setEnabled(connected);

    ui->connexion_ok->setChecked(connected);

    ui->connexion->setEnabled(!connected);
}

void MainWindow::on_middle_pressed()
{
    christobal->beyblade();
}
