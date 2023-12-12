#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QDebug>


#define BIND_PORT 12345
#define BIND_PORT_USER 12346

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void InitUserSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
    void SendUserDatagram(QString data);
    void ReadUserDatagram(QNetworkDatagram datagram);


private slots:
    void readPendingDatagrams(void);
    void readUserPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* serviceUserUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_senderAddress_rdy(QHostAddress sender_address, qint64 data_size);

};

#endif // UDPWORKER_H
