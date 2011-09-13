#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>
#include "Tree.h"
#include "ItemDialog.h"

class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    private:
        QMenu *menuIcons;
        QAction *actionNone, *actionProgress, *actionFailure, *actionSuccess, *actionAdd, *actionDelete;
        Tree *pTree;
        ItemDialog *pItemDial;
    protected:
        // overriden methods
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
    public slots:
        void on_itemChanged(QTreeWidgetItem* item,int column);
        void on_itemCollapsed();
        void on_itemExpanded();
        void deleteItem(QTreeWidgetItem* item);
    public:
        QCustomTreeWidget(QWidget *parent=NULL);
        ~QCustomTreeWidget();
        // associating a tree to the widget
        void setTree(Tree *tree);
        // associating an icon to a state
        static QIcon icon(Item::State state);
};

#endif
