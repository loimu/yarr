#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#endif

#include <QWidget>

#include "ui_aboutdialog.h"

class AboutDialog : public QWidget
{
    Q_OBJECT

    Ui::AboutDialog ui;
    void addText();

public:
    AboutDialog();
};

#endif // ABOUTDIALOG_H
