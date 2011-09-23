#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>

class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        QMenu *menuColumn;
        QAction *actionNone, *actionAddColumn, *actionRemoveColumn, *actionAddRow, *actionRemoveRow;

    protected:
        void mousePressEvent(QMouseEvent *e);

    public:
        QCustomTableWidget(QWidget *parent=NULL);
        ~QCustomTableWidget();
};

#endif
