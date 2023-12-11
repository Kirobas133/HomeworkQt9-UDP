#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}
void UDPworker::InitUserSocket(){
    serviceUserUdpSocket = new QUdpSocket(this);
    serviceUserUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT_USER);
    connect(serviceUserUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readUserPendingDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::SendUserDatagram(QString data){
    QByteArray dataToSend;
    QDataStream outStream(&dataToSend, QIODevice::WriteOnly);
    outStream << data;
    serviceUserUdpSocket->writeDatagram(dataToSend, QHostAddress::LocalHost, BIND_PORT_USER);
}

void UDPworker::readUserPendingDatagrams(void){
    while(serviceUserUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadUserDatagram(datagram);
    }
}

void UDPworker::ReadUserDatagram(QNetworkDatagram datagram){
    QByteArray data;
    data = datagram.data();
    qsizetype userDataSize = data.size();
    qint64 dataSize = userDataSize;
    QHostAddress senderAddress = datagram.senderAddress();
    emit sig_senderAddress_rdy(senderAddress, dataSize);
}


