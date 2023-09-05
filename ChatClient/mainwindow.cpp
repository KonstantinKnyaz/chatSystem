#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _tcpW(new TcpWorker(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Local Chat");
    connect(_tcpW, &TcpWorker::incomeMsg, this, &MainWindow::newMsg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendBtn_clicked()
{
    sendToServer(ui->msgLine->text());
    ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + ui->msgLine->text() + "</font>");
    ui->msgLine->clear();
}


void MainWindow::on_msgLine_returnPressed()
{
    sendToServer(ui->msgLine->text());
    ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + ui->msgLine->text() + "</font>");
    ui->msgLine->clear();
}

void MainWindow::newMsg(QString msg)
{
    ui->textBox->append("<font color=red>Входящее: </font><font color=white>" + msg + "</font>");
}

void MainWindow::sendToServer(const QString &msg)
{
    _tcpW->sentToServer(msg);
}

