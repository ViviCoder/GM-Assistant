/*************************************************************************
* Copyright © 2019 Vincent Prat & Simon Nicolas
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

#include "RenameNoteDialog.h"
#include <QMessageBox>
#include <QFileDialog>

RenameNoteDialog::RenameNoteDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

QString RenameNoteDialog::text() const
{
    return editName->text();
}

void RenameNoteDialog::on_pushEdit_clicked()
{
    if (editName->text()!="")
    {
        accept();
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("renameNoteDialog","Incomplete data",0),QApplication::translate("renameNoteDialog","You must fill the title before validating.",0));
    }
}

int RenameNoteDialog::exec(QString text)
{
    editName->setText(text);
    editName->setFocus();
    return QDialog::exec();
}

void RenameNoteDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}
