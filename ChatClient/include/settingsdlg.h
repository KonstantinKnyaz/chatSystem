#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>

namespace Ui {
class SettingsDlg;
}

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget *parent = nullptr);
    ~SettingsDlg();

private:
    Ui::SettingsDlg *ui;
    QString _ip, _port;
    QString _ipRegExp = "^((25[0-5]|(2[0-4]|1[0-9]|[1-9]|)[0-9])(\\.(?!$)|$)){4}$";
    QString _portRegExp = "^([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$";
};

#endif // SETTINGSDLG_H
