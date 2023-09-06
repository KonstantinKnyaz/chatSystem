#include "settingsdlg.h"
#include "ui_settingsdlg.h"

#include <QRegularExpression>
#include <QValidator>

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{
    ui->setupUi(this);
    QRegularExpression rx(_ipRegExp);
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    ui->ipEdit->setValidator(validator);

    QRegularExpression rxP(_portRegExp);
    QValidator *validatorP = new QRegularExpressionValidator(rxP, this);
    ui->portEdit->setValidator(validatorP);
}

SettingsDlg::~SettingsDlg()
{
    delete ui;
}
