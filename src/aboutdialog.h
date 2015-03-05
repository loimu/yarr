#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>

#include "ui_aboutdialog.h"

class AboutDialog : public QWidget
{
    Q_OBJECT

    Ui::AboutDialog ui;
    void addText();

public:
    AboutDialog(QWidget *parent = 0);
};

#endif // ABOUTDIALOG_H
