#include "tcpworker.h"
#include <QTime>
#include <QFile>
#include <QFileInfo>

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
, _socket(new QTcpSocket(this))
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
                QFile file;
                file.setFileName("C:/Users/knyazev.kp/Pictures/Camera Roll/"+_fileName);
                if(!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                    qWarning() << "Не удалось записать файл";
                    return;
                }
                _fileByte.remove(0, 4);
                file.write(_fileByte);
                file.close();
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
            QString host;
            in >> host >> str;
            emit incomeMsg(host, str);
        }

        nextBlockSize = 0;
        break;
    }
}

void TcpWorker::slotDisconnected()
{

//    file.setFileName("C:/Users/knyazev.kp/Pictures/Camera Roll/test.jpg");
//    if(!file.open(QIODevice::WriteOnly)) {
//        qWarning() << "Не удалось записать файл";
//        return;
//    }
//    QDataStream in(&_data, QIODevice::ReadOnly);
//    in.setVersion(QDataStream::Qt_6_2);
//    if(in.status() != QDataStream::Ok) {
//        qWarning() << "DataStream error:" << in.status();
//        return;
//    }

//    qint64 fSize;
//    QByteArray arrIn;
//    in >> fSize >> arrIn;
//    file.write(arrIn);
    //    file.close();
}

void TcpWorker::sentToServer(const QString ip, const QString &msg, const QString &fileName)
{
    if(!msg.isEmpty())
        if(!sendMsg(ip, msg))
            qWarning() << "Не удалось отправить сообщение";
    if(!fileName.isEmpty())
        if(!sendFile(ip, fileName))
            qWarning() << "Не удалось отправить файл";
}

bool TcpWorker::sendMsg(const QString ip, const QString &msg)
{
    _data.clear();
    int res = 0;
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    QString type = "msg";
    out << quint16(0) << type << ip << _hostName << msg;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    res = _socket->write(_data);
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    return true;
}

bool TcpWorker::sendFile(const QString ip, const QString &fileName)
{
    _data.clear();
    int res = 0;
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось прочитать файл";
        return false;
    }
    QFileInfo info(file);
    qint64 fileSize = file.size();
    QString fName = info.fileName();
    QString type = "file";
    out << quint16(0) << type << fileSize << fName << ip;
    out.device()->seek(0);
    out << quint16(_data.size() - sizeof(quint16));
    res = _socket->write(_data);
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    _data.clear();

    QDataStream outFile(&_data, QIODevice::WriteOnly);
    outFile << quint16(0) << file.readAll();
    outFile.device()->seek(0);
    outFile << quint16(_data.size() - sizeof(quint16));
    res = _socket->write(_data);
    file.close();
    if(res == -1)
        return false;
    _socket->waitForBytesWritten(5000);
    return true;
}
