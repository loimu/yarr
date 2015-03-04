#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class QSettings;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
    QSettings *settings;

private slots:
    void on_buttonBox_accepted();
};

#endif // SETTINGS_H
