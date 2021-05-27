#ifndef MYROBOT_H
#define MYROBOT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include <QMutex>

class MyRobot : public QObject {
    Q_OBJECT
<<<<<<< HEAD

=======
>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4
public:
    explicit MyRobot(QObject *parent = 0);
    void doConnect();
    void disConnect();
<<<<<<< HEAD
=======

>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;

<<<<<<< HEAD
    void move();
    void decreasedSpeed();
    void forward();
    void backward();
    void left();
    void right();

    quint16 Crc16(QByteArray Adresse_tab);
    bool isConnected();
    int getSpeed();
    void setMaxSpeed(int max_speed);
    void beyblade();
=======
    void forward();
    void backward();
    short Crc16(QByteArray Adresse_tab , unsigned char Taille_max);
>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4

signals:
    void updateUI(const QByteArray Data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();

private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;
<<<<<<< HEAD

    bool is_connected;
    int nb_timer_to_wait;
    const unsigned char v_max = 240; // = 240 tics max
    unsigned char _max_speed;
    bool go_forward;
    bool go_backward;
    bool go_left;
    bool go_right;
    bool _beyblade;
=======
>>>>>>> 5ceaa2168f8ac92b753591c572b643fc29f162c4
};

#endif // MYROBOT_H
