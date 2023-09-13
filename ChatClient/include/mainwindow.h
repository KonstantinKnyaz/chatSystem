#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "tcpworker.h"
#include "settingsdlg.h"
#include "clienttablemodel.h"
#include "addclientdlg.h"

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

    void newMsg(QString host, QString msg);

    void newFile(QString host, QString fileName, QByteArray data);

    void on_addClient_clicked();

    void on_delClient_clicked();

    void on_addFile_clicked();

    void getLink(const QUrl &url);

private:
    Ui::MainWindow *ui;
    TcpWorker *_tcpW = Q_NULLPTR;
    SettingsDlg *_sDlg = Q_NULLPTR;
    AddClientDlg *_cDlg = Q_NULLPTR;
    ClientTableModel *model = Q_NULLPTR;
    QString _hostName;
    QString _currClientIp;
    QString _fileName = Q_NULLPTR;
    QHash<QString,QByteArray> *filesSpis = Q_NULLPTR;

    void sendToServer();

    int saveFile(const QString &fileName);
};
#endif // MAINWINDOW_H
