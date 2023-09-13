#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QMutex>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server();

public slots:
    void incomingConnection(qintptr handle);
    void slotReadyRead();
//    QTcpSocket *nextPendingConnection();

private:
    QTcpSocket *_socket = Q_NULLPTR;
    QHash<QString,QTcpSocket*> *_Sockets = Q_NULLPTR;
    QByteArray _data;
    quint16 nextBlockSize = 0;
    QString _hostName;
    QByteArray _fileByte;
    qint64 _fileSize = 0;
    QString _fileName;
    QString _msgType = "msg";
    QString _fIp;
    QMutex _mutex;

    bool sendToClient(QString &ip, QString &msg);

    bool sendFileToClient();
};

#endif // SERVER_H
