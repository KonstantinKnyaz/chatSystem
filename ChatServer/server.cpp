#include "server.h"
#include <QTime>

Server::Server() :
_Sockets(new QHash<QString,QTcpSocket*>())
{
    if(listen(QHostAddress::Any, 2323)) {
        qInfo() << "Сервер запущен";
    } else {
        qCritical() << "Ошибка при запуске сервера";
    }
}

void Server::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    QStringList ipLst = socket->peerAddress().toString().split(":");
    qDebug() << ipLst;
    _Sockets->insert(ipLst.last(), socket);

    qInfo() << "Client connected: " << socket->peerAddress() << socket->peerPort();
}

void Server::slotReadyRead()
{
    _socket = (QTcpSocket*)sender();
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() != QDataStream::Ok) {
        qWarning() << "DataStream error:" << in.status();
        return;
    }
    while(true) {
        if(nextBlockSize == 0) {
            qDebug() << "nextBlockSize = 0";
            if(_socket->bytesAvailable() < 2) {
                qDebug() << "_data < 2";
                break;
            }
            in >> nextBlockSize;
            qDebug() << "nextBlockSize -" << nextBlockSize;
        }
        if(_socket->bytesAvailable() < nextBlockSize) {
            qDebug() << "data not full";
            break;
        }
        QString str;
        QString ip;
        in >> ip >> _hostName >> str;
        nextBlockSize = 0;
        qDebug() << ip << _hostName << " " << str;
        sendToClient(ip, str);
        break;
    }
}

void Server::sendToClient(QString &ip, QString &msg)
{
    _data.clear();
    _socket = _Sockets->value(ip);
    if(_socket == Q_NULLPTR) {
        qWarning() << "Не найден пользователь с таким ip:" << ip;
        return;
    }
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << _hostName << msg;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    _socket->write(_data);
}
