#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpSocket>

class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(QObject *parent = Q_NULLPTR);
    ~TcpWorker();

    void sentToServer(const QString &msg);

public slots:
    void slotReadyRead();
    void slotDisconnected();

signals:
    void incomeMsg(QString msg);

private:
    QTcpSocket *_socket = Q_NULLPTR;
    QByteArray _data;
    quint16 nextBlockSize = 0;
};

#endif // TCPWORKER_H
