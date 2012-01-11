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

#include "ItemDialog.h"
#include <QMessageBox>
#include <QFileDialog>

ItemDialog::ItemDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

Item::State ItemDialog::state() const
{
    if (radioNone->isChecked())
        return Item::sNone;
    else if (radioProgress->isChecked())
        return Item::sProgress;
    else if (radioFailure->isChecked())
        return Item::sFailure;
    else
        return Item::sSuccess;
}

QString ItemDialog::text() const
{
    return editItem->text();
}

Item::Type ItemDialog::type() const
{
    if (radioBasic->isChecked())
        return Item::tBasic;
    else if (radioSound->isChecked())
        return Item::tSound;
    else if (radioPicture->isChecked())
        return Item::tPicture;
}

QString ItemDialog::fileName() const
{
    return editFile->text();
}

void ItemDialog::on_pushChild_clicked()
{
    if (editItem->text()!="")
    {
        if (radioBasic->isChecked() || editFile->text()!="")
        {
            rRes = rChild;
            accept();
        }
        else
        {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must select a file before validating.",0));
        }
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}

void ItemDialog::on_pushBrother_clicked()
{
    if (editItem->text()!="")
    {
        if (radioBasic->isChecked() || editFile->text()!="")
        {
            rRes = rBrother;
            accept();
        }
        else
        {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must select a file before validating.",0));
        }
    }
    else
    {
        QMessageBox::critical(this,QApplication::translate("itemDialog","Uncomplete data",0),QApplication::translate("itemDialog","You must fill the content before validating.",0));
    }
}

ItemDialog::Result ItemDialog::selectionResult() const
{
    return rRes;
}

void ItemDialog::on_radioBasic_clicked()
{
    toolBrowse->setEnabled(false);
}

void ItemDialog::on_radioSound_clicked()
{
    toolBrowse->setEnabled(true);
}

void ItemDialog::on_radioPicture_clicked()
{
    toolBrowse->setEnabled(true);
}

void ItemDialog::on_toolBrowse_clicked()
{
    switch (type())
    {
        case Item::tSound:  {
                                editFile->setText(QFileDialog::getOpenFileName(this,QApplication::translate("itemDialog","Select the sound file to associate to the item",0),"",QApplication::translate("itemDialog","Audio files (*.mp3 *.wav *.ogg)",0)));    
                            }
        case Item::tPicture:  { 
                                editFile->setText(QFileDialog::getOpenFileName(this,QApplication::translate("itemDialog","Select the picture file to associate to the item",0),"",QApplication::translate("itemDialog","Picture files (*.jpg *.jpeg *.png *.bmp)",0)));    
                            }
        default:            break;
    }
}

int ItemDialog::exec()
{
    // resets the window
    radioNone->setChecked(true);
    radioBasic->setChecked(true);
    toolBrowse->setEnabled(false);

    editItem->setText("");
    editItem->setFocus();
    editFile->setText("");

    // shows it
    return QDialog::exec();
}
