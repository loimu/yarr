#ifndef PAINTER_H_
#define PAINTER_H_

#include <QApplication>
#include <QPainter>
#include <QItemDelegate>
#include "board.h"

class Painter: public QItemDelegate {
Q_OBJECT
public:
    Painter(QWidget *parent = 0);
    virtual ~Painter();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /* PAINTER_H_ */
