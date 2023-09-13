#include "server.h"
#include <QTime>
#include <QFile>

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
        qDebug() << "bytesAvailable: " << _socket->bytesAvailable();

        //        if(_socket->bytesAvailable() < nextBlockSize) {
        //            qDebug() << "data not full";
        //            break;
        //        }

        if(_msgType == "file") {
            qDebug() << "Запись файла...";
            QByteArray ar = _socket->readAll();
            _fileByte += ar;
            qDebug() << "размер байтов: " << _fileByte.size();
            if(_fileByte.size() >= _fileSize) {
                _fileByte.remove(0, 4);
                if(!sendFileToClient())
                    qWarning() << "Не удалось отправить файл";
                _fileByte.clear();
                _msgType = "msg";
                _fileSize = 0;
                _fileName.clear();
                _fIp.clear();
            }
            break;
        }

        in >> _msgType;
        if(_msgType == "file") {
            in >> _fileSize >> _fileName >> _fIp;
            qDebug() << _fileSize << _fileName << _fIp;
        } else {
            QString str;
            QString ip;
            in >> ip >> _hostName >> str;
            qDebug() << ip << _hostName << " " << str;
            if(!sendToClient(ip, str))
                qWarning() << "Не удалось отправить сообщение";
        }

        nextBlockSize = 0;
        break;
    }
}

bool Server::sendToClient(QString &ip, QString &msg)
{
    _data.clear();
    _socket = _Sockets->value(ip);
    if(_socket == Q_NULLPTR) {
        qWarning() << "Не найден пользователь с таким ip:" << ip;
        return false;
    }
    QString type = "msg";
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << quint16(0) << type << _hostName << msg;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    int res = _socket->write(_data);
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    return true;
}

bool Server::sendFileToClient()
{
    _data.clear();
    _socket = _Sockets->value(_fIp);
    if(_socket == Q_NULLPTR) {
        qWarning() << "Не найден пользователь с таким ip:" << _fIp;
        return false;
    }

    int res = 0;
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    qint64 fileSize = _fileByte.size();
    QString fName = _fileName;
    QString type = "file";
    out << quint16(0) << type << fileSize << fName << _fIp;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    res = _socket->write(_data);
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    _data.clear();

    QDataStream outFile(&_data, QIODevice::WriteOnly);
    outFile << quint16(0) << _fileByte;
    outFile.device()->seek(0);
    outFile << quint16(_data.size() - sizeof(quint16));
    res = _socket->write(_data);
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    return true;
}
