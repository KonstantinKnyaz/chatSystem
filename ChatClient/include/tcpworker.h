#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpSocket>

class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(QString &hostName, QString &ip, quint16 port, QObject *parent = Q_NULLPTR);
    explicit TcpWorker(QString &hostName, QObject *parent = Q_NULLPTR);
    ~TcpWorker();

    void sentToServer(const QString ip, const QString &msg);

public slots:
    void slotReadyRead();
    void slotDisconnected();

signals:
    void incomeMsg(QString host, QString msg);

private:
    QTcpSocket *_socket = Q_NULLPTR;
    QByteArray _data;
    quint16 nextBlockSize = 0;
    QString _hostName;
};

#endif // TCPWORKER_H