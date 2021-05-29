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
public:
    explicit MyRobot(QObject *parent = 0);
    void doConnect();
    void disConnect();
    QByteArray DataToSend;
    QByteArray DataReceived;
    QMutex Mutex;

    void move();
    void decreasedSpeed();
    void forward();
    void backward();
    void left();
    void right();
    quint16 Crc16(QByteArray Adresse_tab);
    bool isConnected();
    int getSpeed();
    void beyblade();

signals:
    void updateUI(const QByteArray Data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void MyTimerSlot();
    void setMaxSpeed(int max_speed);

private:
    QTcpSocket *socket;
    QTimer *TimerEnvoi;

    const unsigned char v_max = 240; // = 240 tics max
    unsigned char _max_speed;

    bool is_connected;
    int nb_timer_to_wait;

    bool go_forward;
    bool go_backward;
    bool go_left;
    bool go_right;
    bool _beyblade;
};

#endif // MYROBOT_H
