#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>
#include "Tree.h"
#include "ItemDialog.h"
#include "SoundEngine.h"

class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    private:
        QMenu *menuIcons;
        QAction *actionNone, *actionProgress, *actionFailure, *actionSuccess, *actionAdd, *actionDelete;
        Tree *pTree;
        SoundEngine *pSoundEngine;
        bool bPlayMusic;
        ItemDialog *pItemDial;
        QTreeWidgetItem *dragSource;
    protected:
        // overriden methods
        void mouseDoubleClickEvent(QMouseEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
        void dragEnterEvent(QDragEnterEvent *e);
        void dropEvent(QDropEvent *e);
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
        void setSoundEngine(SoundEngine *soundEngine, bool playMusic=false);
        // associating an icon to a state
        static QIcon icon(Item::State state);
};

#endif
