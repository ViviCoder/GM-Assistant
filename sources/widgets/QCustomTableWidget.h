#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include "ChangeHeaderDialog.h"

class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        QMenu *menuColumn;
        QAction *actionNone, *actionAddColumn, *actionRemoveColumn, *actionAddRow, *actionRemoveRow, *actionEditRow, *actionEditColumn;
        ChangeHeaderDialog *pChangeHeaderDial;

    protected:
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);

    public:
        QCustomTableWidget(QWidget *parent=NULL);
        ~QCustomTableWidget();
};

#endif
