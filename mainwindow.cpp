#include <QMessageBox>
#include <QWebEngineView>
#include <QKeyEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myrobot.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->grabKeyboard();
    _forward = false;
    _backward = false;
    _left = false;
    _right = false;
    _beyblade = false;
    _robot_mode=true;

    ui->setupUi(this);
    setWindowTitle("Christobal");
    display(false);

    christobal = new MyRobot (this);

    view = new QWebEngineView (this);
    ui->camera->addWidget(view);
    view->load(QUrl("http://192.168.1.106:8080/?action=stream"));
    view->hide();

    url = new QWebEngineView (this);
    url->hide();

    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), ui->v_max_aff, SLOT(display(int)));
    connect(ui->v_max_slider, SIGNAL(valueChanged(int)), this->christobal, SLOT(setMaxSpeed(int)));
    connect(this->christobal, SIGNAL(changeConnectState(bool)), this, SLOT(display(bool)));
    connect(this->christobal, SIGNAL(changeConnectState(bool)), this, SLOT(displayCamera(bool)));
    connect(this->christobal, SIGNAL(updateUI(QByteArray)), this, SLOT(reloadDisplay(QByteArray)));
    connect(this->christobal, SIGNAL(changeSpeed(int)), ui->vitesse_aff, SLOT(display(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Connexion / Déconnexion
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

// Affiche boutons et vidéo caméra
void MainWindow::display(bool connected)
{
    _connected = connected;

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
    ui->batterieBar->setEnabled(connected);
    ui->batterielabel->setEnabled(connected);

    ui->connexion_ok->setChecked(connected);

    ui->connexion->setEnabled(!connected);

    // Display infos capteurs
    ui->av_g_ok_label->show();
    ui->av_g_attention_label->hide();
    ui->av_g_stop_label->hide();

    ui->av_d_ok_label->show();
    ui->av_d_attention_label->hide();
    ui->av_d_stop_label->hide();

    ui->arr_g_ok_label->show();
    ui->arr_g_attention_label->hide();
    ui->arr_g_stop_label->hide();

    ui->arr_d_ok_label->show();
    ui->arr_d_attention_label->hide();
    ui->arr_d_stop_label->hide();
}
void MainWindow::displayCamera(bool display)
{
    if (display)
    {
        view->show();
    } else view->hide();
}

// Changement de l'affichage avec le retour
void MainWindow::reloadDisplay(QByteArray retour)
{
    changeBat(retour[2]); // batterie
    changeCapteur(retour[3], retour[11], retour[12], retour[4]);
}

// Change l'affichage du niveau de batterie et des capteurs
void MainWindow::changeBat(unsigned char bat)
{
    int bat_int= (int) bat;
    bat_int-=3;
    if (bat_int > 124)
    {
        bat_int=124;
    }
    else if (bat_int<0)
    {
        bat_int=0;
    }
    bat_int=bat_int*100/124;
    ui->batterieBar->setValue(bat_int);
}
void MainWindow::changeCapteur(unsigned char av_g, unsigned char av_d, unsigned char arr_g, unsigned char arr_d)
{
    int av_g_int= (int) av_g;
    int av_d_int= (int) av_d;
    int arr_g_int= (int) arr_g;
    int arr_d_int= (int) arr_d;

    // Avant gauche
    if (av_g_int>180)
    {
        ui->av_g_ok_label->hide();
        ui->av_g_attention_label->hide();
        ui->av_g_stop_label->show();
    }
    else if (av_g_int>150)
    {
        ui->av_g_ok_label->hide();
        ui->av_g_attention_label->show();
        ui->av_g_stop_label->hide();
    }
    else if (av_g_int<150)
    {
        ui->av_g_ok_label->show();
        ui->av_g_attention_label->hide();
        ui->av_g_stop_label->hide();
    }

    // Avant droit
    if (av_d_int>180)
    {
        ui->av_d_ok_label->hide();
        ui->av_d_attention_label->hide();
        ui->av_d_stop_label->show();
    }
    else if (av_d_int>150)
    {
        ui->av_d_ok_label->hide();
        ui->av_d_attention_label->show();
        ui->av_d_stop_label->hide();
    }
    else if (av_d_int<150)
    {
        ui->av_d_ok_label->show();
        ui->av_d_attention_label->hide();
        ui->av_d_stop_label->hide();
    }

    // Arriere gauche
    if (arr_g_int>180)
    {
        ui->arr_g_ok_label->hide();
        ui->arr_g_attention_label->hide();
        ui->arr_g_stop_label->show();
    }
    else if (arr_g_int>150)
    {
        ui->arr_g_ok_label->hide();
        ui->arr_g_attention_label->show();
        ui->arr_g_stop_label->hide();
    }
    else if (arr_g_int<150)
    {
        ui->arr_g_ok_label->show();
        ui->arr_g_attention_label->hide();
        ui->arr_g_stop_label->hide();
    }

    // Arriere droit
    if (arr_d_int>180)
    {
        ui->arr_d_ok_label->hide();
        ui->arr_d_attention_label->hide();
        ui->arr_d_stop_label->show();
    }
    else if (arr_d_int>150)
    {
        ui->arr_d_ok_label->hide();
        ui->arr_d_attention_label->show();
        ui->arr_d_stop_label->hide();
    }
    else if (arr_d_int<150)
    {
        ui->arr_d_ok_label->show();
        ui->arr_d_attention_label->hide();
        ui->arr_d_stop_label->hide();
    }
}

// Récupération des touches pressées et relachées
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (_connected)
    {
        if(event->key() == Qt::Key_Up)
        {
            _forward=true;
        }
        else if (event->key() == Qt::Key_Left)
        {
            _left=true;
        }
        else if (event->key() == Qt::Key_Right)
        {
            _right=true;
        }
        else if (event->key() == Qt::Key_Down)
        {
            _backward=true;
        }
        else if (event->key() == Qt::Key_Space)
        {
            changeMode();
        }
        send();
    }
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())
    {
         return;
    }
    else if (_connected)
    {
        if(event->key() == Qt::Key_Up)
        {
            _forward=false;
        }
        else if (event->key() == Qt::Key_Left)
        {
            _left=false;
        }
        else if (event->key() == Qt::Key_Right)
        {
            _right=false;
        }
        else if (event->key() == Qt::Key_Down)
        {
            _backward=false;
        }
    }
    send();
}

// Fonction de déplacement
// Avancer
void MainWindow::on_forward_pressed()
{
    if (_robot_mode)
    {
        goForward();
    } else cameraUp();
}
void MainWindow::goForward()
{
    christobal->forward();
}

// Reculer
void MainWindow::on_backward_pressed()
{
    if (_robot_mode)
    {
        goBackward();
    } else cameraDown();

}
void MainWindow::goBackward()
{
    christobal->backward();
}

// Avancer gauche
void MainWindow::on_forward_left_pressed()
{
    if (_robot_mode)
    {
        goForwardLeft();
    }
    else
    {
        cameraUp();
        cameraLeft();
    }
}
void MainWindow::goForwardLeft()
{
    christobal->left(true);
}

// Avacner droite
void MainWindow::on_forward_right_pressed()
{
    if (_robot_mode)
    {
        goForwardRight();
    }
    else
    {
        cameraUp();
        cameraRight();
    }
}
void MainWindow::goForwardRight()
{
    christobal->right(true);
}

// Reculer gauche
void MainWindow::on_backward_left_pressed()
{
    if (_robot_mode)
    {
        goBackwardLeft();
    }
    else
    {
        cameraDown();
        cameraLeft();
    }
}
void MainWindow::goBackwardLeft()
{
    christobal->left(false);
}

// Reculer droite
void MainWindow::on_backward_right_pressed()
{
    if (_robot_mode)
    {
        goBackwardRight();
    }
    else
    {
        cameraDown();
        cameraRight();
    }
}
void MainWindow::goBackwardRight()
{
    christobal->right(false);
}

// Toupie gauche
void MainWindow::on_left_pressed()
{
    if (_robot_mode)
    {
        goBeybladeLeft();
    } else cameraLeft();
}
void MainWindow::goBeybladeLeft()
{
    christobal->beyblade(true);
}

// Toupie droite
void MainWindow::on_right_pressed()
{
    if (_robot_mode)
    {
        goBeybladeRight();
    } else cameraRight();
}
void MainWindow::goBeybladeRight()
{
    christobal->beyblade(false);
}

// Changement de mode !
void MainWindow::on_middle_pressed()
{
    changeMode();
}
void MainWindow::changeMode()
{
    if (_robot_mode)
    {
        ui->mode->setText("Mode : Caméra");
    } else ui->mode->setText("Mode : Robot");
    _robot_mode=!_robot_mode;
}

// Envoie la commande appropriée
void MainWindow::send()
{
    if ((_forward && !_backward) || (!_forward && _backward))
    {
        if (_forward)
        {
            if (_robot_mode)
            {
                if ((_left && _right) || (!_left && !_right))
                {
                    goForward();
                }
                else if (_left)
                {
                    goForwardLeft();
                }
                else if (_right)
                {
                    goForwardRight();
                }
            } else cameraUp();
        }
        else if (_backward)
        {
            if (_robot_mode)
            {
                if ((_left && _right) || (!_left && !_right))
                {
                    goBackward();
                }
                else if (_left)
                {
                    goBackwardLeft();
                }
                else if (_right)
                {
                    goBackwardRight();
                }
            } else cameraDown();
        }
    }
    else if ((_left && !_right) || (!_left && _right))
    {
        if (_left)
        {
            if (_robot_mode)
            {
                goBeybladeLeft();
            } else cameraLeft();
        }
        else if (_right)
        {
            if (_robot_mode)
            {
                goBeybladeRight();
            } else cameraRight();
        }
    }
    else if (christobal->getSpeed()>0)
    {
        christobal->decreasedSpeed();
    }
}

void MainWindow::cameraUp()
{
    url->load(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-255"));
}

void MainWindow::cameraDown()
{
    url->load(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=255"));
}

void MainWindow::cameraLeft()
{
    url->load(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=255"));
}

void MainWindow::cameraRight()
{
    url->load(QUrl("http://192.168.1.106:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-255"));
}
