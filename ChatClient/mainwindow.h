#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "tcpworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendBtn_clicked();

    void on_msgLine_returnPressed();

    void newMsg(QString msg);

private:
    Ui::MainWindow *ui;
    TcpWorker *_tcpW = Q_NULLPTR;
    void sendToServer(const QString &msg);
};
#endif // MAINWINDOW_H
