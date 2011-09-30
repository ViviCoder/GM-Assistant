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

#ifndef HEADER_QCUSTOMTREEWIDGETITEM
#define HEADER_QCUSTOMTREEWIDGETITEM

#include <QTreeWidgetItem>
#include "QCustomTreeWidget.h"
#include "Branch.h"

class QCustomTreeWidgetItem: public QTreeWidgetItem
{
    private:
        Branch* pBranch;
    public:
        QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch);
        QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch);
        QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch, QCustomTreeWidgetItem *previous);
        QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch, QCustomTreeWidgetItem *previous);
        Branch* branch();
        // update the displat (text and icons)
        void updateDisplay();
};

#endif
