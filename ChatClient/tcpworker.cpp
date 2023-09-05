#include "tcpworker.h"
#include <QTime>

TcpWorker::TcpWorker(QObject *parent) : QObject(parent)
  ,_socket(new QTcpSocket(this))
{
    _socket->connectToHost(QHostAddress::LocalHost, 2323);
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
        in >> str;
        emit incomeMsg(str);
        nextBlockSize = 0;
        break;
    }
}

void TcpWorker::slotDisconnected()
{

}

void TcpWorker::sentToServer(const QString &msg)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << QTime::currentTime() << msg;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    _socket->write(_data);
}
