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

#include "QCustomTreeWidgetItem.h"
#include "ItemFactory.h"

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch): QTreeWidgetItem(parent), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidget *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
    updateDisplay();
}

QCustomTreeWidgetItem::QCustomTreeWidgetItem(QCustomTreeWidgetItem *parent, Branch *branch, QCustomTreeWidgetItem *previous): QTreeWidgetItem(parent,previous), pBranch(branch)
{
    updateDisplay();
}

Branch* QCustomTreeWidgetItem::branch()
{
    return pBranch;
}

void QCustomTreeWidgetItem::updateDisplay()
{
    Item *item = pBranch->item();
    setText(0,item->content().c_str());
    setIcon(1,QCustomTreeWidget::icon(item->state()));
    if (item->type()==Item::tSound)
    {
        setIcon(0,QIcon(":/data/images/speaker.png"));
        setToolTip(0,dynamic_cast<SoundItem*>(item)->fileName().c_str()); 
        setStatusTip(0,QApplication::translate("customTree","Double click to play the file",0));
    }
    else if (item->type()==Item::tPicture)
    {
        setIcon(0,QIcon(":/data/images/file_picture.png"));
        setToolTip(0,dynamic_cast<PictureItem*>(item)->fileName().c_str()); 
        setStatusTip(0,QApplication::translate("customTree","Double click to show the file",0));
    }
}
