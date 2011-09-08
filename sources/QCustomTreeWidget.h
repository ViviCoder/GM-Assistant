#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>

class QCustomTreeWidget: public QTreeWidget
{
    protected:
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
    public:
        QCustomTreeWidget(QWidget *parent=NULL);
};

#endif
