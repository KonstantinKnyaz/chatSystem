#include "addclientdlg.h"
#include "ui_addclientdlg.h"

#include <QRegularExpression>
#include <QValidator>

AddClientDlg::AddClientDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClientDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Add Client");
    QRegularExpression rx("^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])(\\.(?!$)|$)){4}$");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->ipEdit->setValidator(validator);
}

AddClientDlg::~AddClientDlg()
{
    delete ui;
}

QString AddClientDlg::getNick()
{
    return _nick;
}

QString AddClientDlg::getIp()
{
    return _ip;
}

void AddClientDlg::on_buttonBox_accepted()
{
    _nick = ui->nickEdit->text();
    _ip = ui->ipEdit->text();
}


void AddClientDlg::on_buttonBox_rejected()
{
    _nick.clear();
    _ip.clear();
}

