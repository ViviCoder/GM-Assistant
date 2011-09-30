/*************************************************************************
* Copyright © 2011 Free Software Fundation
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

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
    signals:
        void musicToPlay(const std::string &fileName);
};

#endif
