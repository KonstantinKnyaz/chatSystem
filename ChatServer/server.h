#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server();

public slots:
    void incomingConnection(qintptr handle);
    void slotReadyRead();

private:
    QTcpSocket *_socket = Q_NULLPTR;
    QHash<quint64,QTcpSocket*> *_Sockets = Q_NULLPTR;
    QByteArray _data;
    quint16 nextBlockSize = 0;

    void sendToClient(QString &msg);
};

#endif // SERVER_H
