// myrobot.cpp

#include "myrobot.h"
#include <iostream>

MyRobot::MyRobot(QObject *parent) : QObject(parent) {
    DataToSend.resize(9);
    DataToSend[0] = 0xFF;
    DataToSend[1] = 0x07;
    DataToSend[2] = 0x0; // speed_front_left_wheel              // check devant et derriere !!!!!!!!!!!!
    DataToSend[3] = 0x0; // speed_behind_left_wheel
    DataToSend[4] = 0x0; // speed_front_right_wheel
    DataToSend[5] = 0x0; // speed_behind_right_wheel
    DataToSend[6] = 0x0; // 80/0x50 : devant, 0/0x0 : recule, 64/0x40 : gauche sur place, 16/0x10 : droit sur place
    DataToSend[7] = 0x0; // NOT USED ON TCP
    DataToSend[8] = 0x0; // NOT USED ON TCP
    DataReceived.resize(21);
    TimerEnvoi = new QTimer();
    // setup signal and slot
    connect(TimerEnvoi, SIGNAL(timeout()), this, SLOT(MyTimerSlot())); //Send data to wifibot timer
}


void MyRobot::doConnect() {
    socket = new QTcpSocket(this); // socket creation
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    qDebug() << "connecting..."; // this is not blocking call
    //socket->connectToHost("LOCALHOST", 15020);
    socket->connectToHost("192.168.1.11", 15020); // connection to wifibot
    // we need to wait...
    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return;
    }
    TimerEnvoi->start(75);

}

void MyRobot::disConnect() {
    TimerEnvoi->stop();
    socket->close();
}

void MyRobot::connected() {
    qDebug() << "connected..."; // Hey server, tell me about you.
}

void MyRobot::disconnected() {
    qDebug() << "disconnected...";
}

void MyRobot::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void MyRobot::readyRead() {
    qDebug() << "reading..."; // read the data from the socket
    DataReceived = socket->readAll();
    emit updateUI(DataReceived);
    qDebug() << DataReceived[0] << DataReceived[1] << DataReceived[2];
}

void MyRobot::MyTimerSlot() {
    qDebug() << "Timer...";
    while(Mutex.tryLock());
    socket->write(DataToSend);
    Mutex.unlock();
}

void MyRobot::forward()
{
    const char v_max = 0xf0; // = 240 tics max
    while(Mutex.tryLock());

    for (int i=2;i<=5;i++)
    {
        if (DataToSend[i]<v_max)
        {
            DataToSend[i]++;
        }
    }
    DataToSend[6]=0x50;
    short crcfull = Crc16(DataToSend, 7);
    DataToSend[7]=crcfull;
    DataToSend[8]=crcfull >> 8;

    Mutex.unlock();
}

void MyRobot::backward()
{
    const char v_min = 0x0; // = 0 tics min
    while(Mutex.tryLock());

    for (int i=2;i<=5;i++)
    {
        if (DataToSend[i]>v_min)
        {
            DataToSend[i]--;
        }
    }
    DataToSend[6]=0x50;
    short crcfull = Crc16(DataToSend, 7);
    DataToSend[7]=crcfull;
    DataToSend[8]=crcfull >> 8;

    Mutex.unlock();
}

short MyRobot::Crc16(QByteArray Adresse_tab , unsigned char Taille_max)
{
    unsigned int Crc = 0xFFFF;
    unsigned int Polynome = 0xA001;
    unsigned int CptOctet = 0;
    unsigned int CptBit = 0;
    unsigned int Parity= 0;

    Crc = 0xFFFF;
    Polynome = 0xA001;
    for ( CptOctet= 0 ; CptOctet < Taille_max ; CptOctet++)
    {
        Crc ^= ( Adresse_tab[CptOctet]);
        for ( CptBit = 0; CptBit <= 7 ; CptBit++)
        {
            Parity= Crc;
            Crc >>= 1;
            if (Parity%2 == true) Crc ^= Polynome;
        }
    }
    return(Crc);
}
