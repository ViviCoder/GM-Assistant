/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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

/*!
 * \brief Custom tree widget
 *
 * This widget is designed to manipulate an underlying Tree
 */
class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    private:
        /*!
         * \brief Pop-up menu
         */
        QMenu *menuIcons;
        /*!
         * \brief Untag action
         */
        QAction *actionNone;
        /*!
         * \brief "In progress" tag action
         */
        QAction *actionProgress;
        /*!
         * \brief "Failed" tag action
         */
        QAction *actionFailure;
        /*!
         * \brief "Succeeded" tag action
         */
        QAction *actionSuccess;
        /*!
         * \brief Add action
         */
        QAction *actionAdd;
        /*!
         * \brief Delete action
         */
        QAction *actionDelete;
        Tree *pTree;
        ItemDialog *pItemDial;
        QTreeWidgetItem *pDragSource;
        bool bNewlySelected;
        bool bEditing;
    protected:
        // overriden methods
        void mouseDoubleClickEvent(QMouseEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
        void dragEnterEvent(QDragEnterEvent *e);
        void dropEvent(QDropEvent *e);
    public slots:
        void on_itemChanged(QTreeWidgetItem* item,int column);
        void on_itemCollapsed();
        void on_itemExpanded();
        void deleteItem(QTreeWidgetItem* item);
        void on_itemSelectionChanged();
    public:
        /*!
         * \brief Constructor of the widget
         * \param parent Parent widget
         *
         * Creates its pop-up menu and connects various slots
         */
        QCustomTreeWidget(QWidget *parent=NULL);
        ~QCustomTreeWidget();
        // associating a tree to the widget
        void setTree(Tree *tree);
        // associating an icon to a state
        static QIcon icon(Item::State state);
    signals:
        void fileToPlay(const std::string &fileName);
};

#endif
