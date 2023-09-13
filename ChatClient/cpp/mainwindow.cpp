#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QHostInfo>
#include <QFileDialog>
#include <QDateTime>
#include <QTextBrowser>
#include <QProcess>

#define MAX_FILE_SIZE 5000000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _cDlg(new AddClientDlg(this))
    , model(new ClientTableModel(this))
    , filesSpis(new QHash<QString,QByteArray>)
    ,_popup(new PopupNotifyWidget(this))
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
    connect(_tcpW, &TcpWorker::incomeFile, this, &MainWindow::newFile);
    connect(ui->textBox, &QTextBrowser::anchorClicked, this, &MainWindow::getLink);
    ui->textBox->setOpenLinks(false);
    ui->textBox->setOpenExternalLinks(false);

    ui->clientTbl->setModel(model);
    ui->clientTbl->horizontalHeader()->setStretchLastSection(true);
    ui->clientTbl->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
    delete ui;
    filesSpis->clear();
    delete filesSpis;
}

void MainWindow::on_sendBtn_clicked()
{
    sendToServer();
}

void MainWindow::on_msgLine_returnPressed()
{
    sendToServer();
}

void MainWindow::newMsg(QString host, QString msg)
{
    ui->textBox->append("<font color=red>"+ host + ": </font><font color=white>" + msg + "</font>");
    _popup->setPopupText(host + ": " + msg);
    _popup->show();
}

void MainWindow::newFile(QString host, QString fileName, QByteArray data)
{
    ui->textBox->append("<font color=red>"+ host + ": </font><font color=white><a href = " + fileName + ">" + fileName + "</a></font>");

    filesSpis->insert(fileName, data);

    _popup->setPopupText(host + ": " + fileName);
    _popup->show();
}

void MainWindow::sendToServer()
{
    if(!ui->clientTbl->currentIndex().isValid())
        return;
    QString ip = ui->clientTbl->model()->index(ui->clientTbl->currentIndex().row(),1, ui->clientTbl->currentIndex()).data().toString();
    _tcpW->sentToServer(ip, ui->msgLine->text(), _fileName);
    if(!ui->msgLine->text().isEmpty())
        ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + ui->msgLine->text() + "</font>");
    ui->msgLine->clear();
    if(!_fileName.isEmpty())
        ui->textBox->append("<font color=cyan> Я: </font><font color=white>" + _fileName + "</font>");
    _fileName.clear();
}

int MainWindow::saveFile(const QString &fileName)
{
    auto it = filesSpis->find(fileName);
    if(it == filesSpis->end())
        return -1;
    QByteArray data = *it;
    QString filePath = QFileDialog::getExistingDirectory(this, Q_NULLPTR, QDir::homePath());
    QFile file;
    file.setFileName(filePath + "\\" + fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось записать файл";
        return -1;
    }
    file.write(data);
    file.close();
    filesSpis->erase(it);
    return 0;
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
    QFile file(_fileName);
    if(file.size() > MAX_FILE_SIZE) {
        qWarning() << "Файл слишком большой";
        _fileName.clear();
        on_addFile_clicked();
    }
}

void MainWindow::getLink(const QUrl &url)
{
    qDebug() << url.toString() << "файл для скачки";
    saveFile(url.toString());
//    QProcess::startDetached(QString("gnome-terminal --working-directory=%1").arg(url.toString()));
}

