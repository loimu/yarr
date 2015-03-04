#include "painter.h"

Painter::Painter(QWidget *parent) : QItemDelegate(parent)
{
}

Painter::~Painter()
{
}

void Painter::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    int statusId = index.data().toInt();
    if(index.data(Qt::BackgroundColorRole).isValid()) {
        painter->fillRect(option.rect, index.data(Qt::BackgroundColorRole).value<QColor> ());
    }
    if(statusId > -1) {
        if(statusId == Board::Black) {
            painter->setBrush(Qt::black);
        } else if(statusId == Board::White) {
            painter->setBrush(Qt::white);
        }
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawEllipse(option.rect.adjusted(3, 3, -3, -3));
    }
}
