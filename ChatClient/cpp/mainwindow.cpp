#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QHostInfo>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _cDlg(new AddClientDlg(this))
    , model(new ClientTableModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Local Chat");

    QSettings settings("./settings.ini", QSettings::IniFormat);
    settings.beginGroup("server");
    QString ip = settings.value("ip").toString();
    quint16 port = 0;
    port = settings.value("port").toUInt();
    settings.endGroup();

    _hostName = QHostInfo::localHostName();

    if(ip.isEmpty())
        _tcpW = new TcpWorker(_hostName, this);
    else
        _tcpW = new TcpWorker(_hostName, ip, port, this);
    connect(_tcpW, &TcpWorker::incomeMsg, this, &MainWindow::newMsg);

    ui->clientTbl->setModel(model);
    ui->clientTbl->horizontalHeader()->setStretchLastSection(true);
    ui->clientTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendBtn_clicked()
{
    if(!ui->clientTbl->currentIndex().isValid())
        return;
    QString ip = ui->clientTbl->model()->index(ui->clientTbl->currentIndex().row(),1, ui->clientTbl->currentIndex()).data().toString();
    sendToServer(ip, ui->msgLine->text());
    ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + ui->msgLine->text() + "</font>");
    ui->msgLine->clear();
}

void MainWindow::on_msgLine_returnPressed()
{
    if(!ui->clientTbl->currentIndex().isValid())
        return;
    QString ip = ui->clientTbl->model()->index(ui->clientTbl->currentIndex().row(),1, ui->clientTbl->currentIndex()).data().toString();
    sendToServer(ip, ui->msgLine->text());
    ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + ui->msgLine->text() + "</font>");
    ui->msgLine->clear();
}

void MainWindow::newMsg(QString host, QString msg)
{
    ui->textBox->append("<font color=red>"+ host + ": </font><font color=white>" + msg + "</font>");
}

void MainWindow::sendToServer(const QString ip, const QString &msg)
{
    _tcpW->sentToServer(ip, msg);
}

void MainWindow::on_addClient_clicked()
{
    if(!_cDlg->exec())
        return;
    model->add(dataModel(_cDlg->getNick(), _cDlg->getIp()));
}

void MainWindow::on_delClient_clicked()
{
    if(ui->clientTbl->currentIndex().isValid())
        model->remove(ui->clientTbl->currentIndex());
}


void MainWindow::on_addFile_clicked()
{
    _fileName = QFileDialog::getOpenFileName(NULL, "Выберите файл для отправки", QDir::homePath(), "*");
    _tcpW->sendFile(_fileName);
}

