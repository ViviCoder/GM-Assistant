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

#ifndef HEADER_ITEMDIALOG
#define HEADER_ITEMDIALOG

#include "ui_ItemDialog.h"
#include "Item.h"

class ItemDialog: public QDialog, private Ui::itemDialog
{
    public:
        // result type
        enum Result {rBrother,rChild};
        // constructor
        ItemDialog(QWidget *parent=NULL);
        // accessors
        Item::State state() const;
        QString text() const;
        Item::Type type() const;
        QString fileName() const;
        Result selectionResult() const;
        void updateDisplay();
    protected:
        void showEvent(QShowEvent *e);
    private:
        Result rRes;

    Q_OBJECT
    public slots:
        void on_pushChild_clicked();
        void on_pushBrother_clicked();
        void on_radioBasic_clicked();
        void on_radioSound_clicked();
        void on_toolBrowse_clicked();
};

#endif
