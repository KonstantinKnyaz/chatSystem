#include "tcpworker.h"
#include <QTime>

#define DEFAULT_IP "10.1.5.60"
#define DEFAULT_PORT 2323

TcpWorker::TcpWorker(QString &hostName, QString &ip, quint16 port, QObject *parent) : QObject(parent)
,_socket(new QTcpSocket(this))
, _hostName(hostName)
{
    if(port == 0)
        port = DEFAULT_PORT;
    _socket->connectToHost(ip, port);
    connect(_socket, &QTcpSocket::readyRead, this, &TcpWorker::slotReadyRead);
}

TcpWorker::TcpWorker(QString &hostName, QObject *parent) : QObject(parent)
, _hostName(hostName)
{
    _socket->connectToHost(DEFAULT_IP, DEFAULT_PORT);
    connect(_socket, &QTcpSocket::readyRead, this, &TcpWorker::slotReadyRead);
//    connect(_socket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnected);
}

TcpWorker::~TcpWorker()
{

}

void TcpWorker::slotReadyRead()
{
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() != QDataStream::Ok) {
        qWarning() << "DataStream error:" << in.status();
        return;
    }
    while(true) {
        if(nextBlockSize == 0) {
            if(_socket->bytesAvailable() < 2) {
                break;
            }
            in >> nextBlockSize;
        }
        if(_socket->bytesAvailable() < nextBlockSize) {
            break;
        }
        QString str;
        QString host;
        in >> host >> str;
        emit incomeMsg(host, str);
        nextBlockSize = 0;
        break;
    }
}

void TcpWorker::slotDisconnected()
{

}

void TcpWorker::sentToServer(const QString ip, const QString &msg)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << ip << _hostName << msg;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    _socket->write(_data);
}
