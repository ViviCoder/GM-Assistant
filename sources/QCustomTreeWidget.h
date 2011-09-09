#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>
#include "Tree.h"

class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    private:
        QMenu *menuIcons;
        QIcon iFailure,iSuccess,iProgress;
        QAction *actionNone, *actionProgress, *actionFailure, *actionSuccess;
        Tree *pTree;
    protected:
        // overriden methods
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
    public slots:
        void on_itemChanged(QTreeWidgetItem* item,int column);
        void on_itemCollapsed();
        void on_itemExpanded();
    public:
        QCustomTreeWidget(QWidget *parent=NULL);
        ~QCustomTreeWidget();
        // associating a tree to the widget
        void setTree(Tree *tree);
};

#endif
