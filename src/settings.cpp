#include <QSettings>

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    settings = new QSettings("yarr", "YARR");
    if(settings->value("main/whiteEnabled").toBool())
        ui->rbWhite->setChecked("true");
    else
        ui->rbBlack->setChecked("true");
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_buttonBox_accepted() {
    if(ui->rbWhite->isChecked())
        settings->setValue("main/whiteEnabled", true);
    else
        settings->setValue("main/whiteEnabled", false);
}
