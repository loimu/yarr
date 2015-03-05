#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, false);
    addText();
}

void AboutDialog::addText() {
    QString text;
    text.append(QString("<p><b>YARR</b> (Yet Another ReveRsi game) v%1"
                        "<br />&copy; 2014-2015 Blaze &lt;blaze@"
                        "open.by&gt;</p><p>Qt %2 (built with Qt %3)<br />"
                        "Licensed under GPL v3 or later.</p><p><b>Links:</b>"
                        "<br />Project: "
                        "<a href=\"https://bitbucket.org/blaze/yarr\">"
                        "https://bitbucket.org/blaze/yarr</a></p>")
                .arg(qApp->applicationVersion())
                .arg(qVersion())
                .arg(QT_VERSION_STR));
    ui.label->setText(text);
}
