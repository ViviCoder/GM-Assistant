/*************************************************************************
* Copyright © 2011-2018 Vincent Prat & Simon Nicolas
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
    Item::Type type = item->type();
    if (Item::is(type, Item::tFile))
    {
        FileItem *fileItem = dynamic_cast<FileItem*>(item);
        std::string fileName = fileItem->fileName(); 
        if (fileItem->isIncluded())
        {
            QFileInfo file(fileName.c_str());
            setToolTip(0, file.fileName() + " (" + QApplication::translate("customTree", "included in the loaded file", 0) + ")");
        }
        else
        {
            setToolTip(0, fileName.c_str()); 
        }
        switch (type)
        {
            case Item::tSound:  setIcon(0,QIcon(":/data/images/speaker.svg"));
                                if (dynamic_cast<QCustomTreeWidget*>(treeWidget())->playingMethod() != QCustomTreeWidget::pmNone)
                                {
                                    setStatusTip(0,QApplication::translate("customTree","Double click to play the file",0));
                                }
                                break;
            case Item::tImage:  setIcon(0,QIcon(":/data/images/image.svg"));
                                setStatusTip(0,QApplication::translate("customTree", "Double click to display the image", 0));
                                break;
            default:    break;
        }
    }
    else if (item->type() == Item::tNote)
    {
        setIcon(0, QIcon(":/data/images/text.svg"));
        setStatusTip(0, QApplication::translate("customTree", "Double click to display the note", 0));
        NoteItem *noteItem = dynamic_cast<NoteItem*>(item);
        setToolTip(0, noteItem->note()->title().c_str());
    }
    else
    {
        setIcon(0, QIcon());
        setStatusTip(0, "");
        setToolTip(0, ""); 
    }
}
