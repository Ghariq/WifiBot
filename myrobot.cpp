// myrobot.cpp

#include "myrobot.h"
#include <iostream>
#include "ui_mainwindow.h"

// Constructeur
MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF; // const
    DataToSend[1] = 0x07; // const
    DataToSend[2] = 0x0; // vitesse gauche
    DataToSend[3] = 0x0; // unused
    DataToSend[4] = 0x0; // vitesse droite
    DataToSend[5] = 0x0; // unused
    DataToSend[6] = 0x0; // 80/0x50 : devant, 0/0x0 : recule, 64/0x40 : gauche sur place, 16/0x10 : droit sur place
    DataToSend[7] = 0x0; // CRC Part 1
    DataToSend[8] = 0x0; // CRC Part 2
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();

    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
    _beyblade=false;
    go_backward=false;
    go_forward=false;
    go_left=false;
    go_right=false;
    _max_speed=v_max;
    nb_timer_to_wait=5;
}

void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    //qDebug() << "Connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.106", 15020); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
    } else TimerEnvoi->start(75);
}

// Fonction de déconnexion
void MyRobot::disConnect()
{
    TimerEnvoi->stop();
    socket->close();
}

// Quand connecté :
void MyRobot::connected() {
    qDebug() << "Connected"; // Hey server, tell me about you.
    emit changeConnectState(true);
}

// Quand déconnecté :
void MyRobot::disconnected() {
    qDebug() << "Disconnected";
    emit changeConnectState(false);
}

// Quand on écrit X octets :
void MyRobot::bytesWritten(qint64 bytes) {
    //qDebug() << bytes << " bytes written...";
}

// Lecture de données
void MyRobot::readyRead() {
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
}

// A chaque timer :
void MyRobot::MyTimerSlot()
{
    if (nb_timer_to_wait>0)
    {
        nb_timer_to_wait--;
    } else decreasedSpeed();
    while (Mutex.try_lock());
    socket->write(DataToSend);
    socket->readAll();
    Mutex.unlock();
}

// Fonction move avant/arriere
void MyRobot::move()
{
    nb_timer_to_wait=2;
    while(Mutex.tryLock()); // On attend la libération du Mutex

    if (!_beyblade)
    {
        for (int i=1; i<3; i++) // On augmente la vitesse
        {
            int nb=2*i;
            if ((unsigned char)DataToSend[nb]<(unsigned char)_max_speed && (go_forward || go_backward)) // Si on a pas atteind la vitesse max et que l'on veut avancer
            {
                if ((unsigned char)DataToSend[nb]<(unsigned char)70) // Si on a une vitesse inférieure à 70 on se place à 70
                {
                    DataToSend[nb]=(unsigned char)70;
                } else DataToSend[nb] = DataToSend[nb] + (unsigned char)10; // On augmente de 10 la vitesse
                if ((unsigned char)DataToSend[nb]>(unsigned char)_max_speed) // Si on a dépassé la vitesse max
                {
                    DataToSend[nb]=(unsigned char)_max_speed;
                }
            }
        }

        // Pour tourner
        if (go_left) // Si il faut aller à gauche
        {
            DataToSend[2]= (unsigned char)(((unsigned char)DataToSend[4])-((unsigned char)70));
        }
        else if (go_right) // Si il faut aller à droite
        {
            DataToSend[4]= (unsigned char)(((unsigned char)DataToSend[2])-((unsigned char)70));
        }

        // Sens des roues
        if (go_backward) // Si on veut aller vers l'arriere
        {
            DataToSend[6]=0x0;
        } else DataToSend[6]=0x50; // Sinon on va vers l'avant
    }
    else
    {
        DataToSend[2]=(unsigned char)_max_speed;
        DataToSend[4]=(unsigned char)_max_speed;
        if (go_right)
        {
            DataToSend[6]=16;
        } else DataToSend[6]=64;
    }

    // Reset des variables de déplacement
    go_left = false;
    go_right = false;
    go_forward=false;
    go_backward=false;
    _beyblade = false;

    // Calcul CRC
    short crcfull = Crc16(DataToSend);
    DataToSend[7]=crcfull;
    DataToSend[8]=crcfull >> 8;

    getSpeed();

    Mutex.unlock();
}

// Arrête/ralenti quand on appuie plus
void MyRobot::decreasedSpeed()
{
    if ((unsigned char)DataToSend[2]>(unsigned char)0)
    {
        DataToSend[2]=0;
    }
    if ((unsigned char)DataToSend[4]>(unsigned char)0)
    {
        DataToSend[4]=0;
    }
    getSpeed();
}

// Fonction avancer
void MyRobot::forward()
{
    go_forward=true;
    move();
}

// Fonction reculer
void MyRobot::backward()
{
    go_backward=true;
    move();
}

// Fonction gauche
void MyRobot::left(bool go_forward)
{
    go_left=true;
    if (go_forward)
    {
        forward();
    } else backward();
}

// Fonction droite
void MyRobot::right(bool go_forward)
{
    go_right=true;
    if (go_forward)
    {
        forward();
    } else backward();
}

// Fonction calcul du CRC
quint16 MyRobot::Crc16(QByteArray tab)
{
    tab.data();
    unsigned char *data = (unsigned char*) tab.constData();
    quint16 crc = 0xFFFF;
    quint16 poly = 0xA001;
    quint16 parity =0;
    int pos=1;
    for (; pos<tab.length()-2; pos++)
    {
        crc ^= *(data+pos);
        for (unsigned int bit=0; bit <=7; bit++)
        {
            parity = crc;
            crc >>=1;
            if (parity%2==true) crc ^= poly;
        }
    }
    return crc;
}

// Retourne la vitesse du plus rapide
int MyRobot::getSpeed()
{
    unsigned char speed;
    if ((unsigned char) DataToSend[2]< (unsigned char) DataToSend[4])
    {
        speed = (unsigned char) DataToSend[4];
    } else speed = (unsigned char) DataToSend [2];
    emit changeSpeed((int) speed);
    return ((int) speed);
}

// Change la vitesse max
void MyRobot::setMaxSpeed(int max_speed)
{
    if (max_speed <= (int)v_max && max_speed >=0)
    {
        _max_speed=(unsigned char) max_speed;
    }
}

// Fait tourner le robot sur lui même
void MyRobot::beyblade(bool right)
{
    go_right=right;
    _beyblade=true;
    move();
}
