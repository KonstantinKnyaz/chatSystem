#ifndef ADDCLIENTDLG_H
#define ADDCLIENTDLG_H

#include <QDialog>

namespace Ui {
class AddClientDlg;
}

class AddClientDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AddClientDlg(QWidget *parent = nullptr);
    ~AddClientDlg();

    QString getNick();
    QString getIp();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddClientDlg *ui;
    QString _nick, _ip;
};

#endif // ADDCLIENTDLG_H
