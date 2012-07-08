/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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
    if (radioSound->isChecked())
        return Item::tSound;
    else if (radioImage->isChecked())
        return Item::tImage;
    else
        return Item::tBasic;
}

QString ItemDialog::fileName() const
{
    QString result(editFile->text()); // file selected
    QString directory(QDir::current().path()); // current directory
    if (result.startsWith(directory))
    {
        return result.replace(0,directory.length()+1,"");
    }

    return result;
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
    editFile->setText("");
}

void ItemDialog::on_radioImage_clicked()
{
    toolBrowse->setEnabled(true);
    editFile->setText("");
}

void ItemDialog::on_toolBrowse_clicked()
{
    switch (type())
    {
        case Item::tSound:  {
                                editFile->setText(QFileDialog::getOpenFileName(this,QApplication::translate("itemDialog","Select the sound file to associate to the item",0),"",QApplication::translate("itemDialog","Audio files (*.mp3 *.wav *.ogg)",0)));    
                                break;
                            }
        case Item::tImage:  { 
                                editFile->setText(QFileDialog::getOpenFileName(this,QApplication::translate("itemDialog","Select the picture file to associate to the item",0),"",QApplication::translate("itemDialog","Image files (*.jpg *.jpeg *.png *.bmp *.svg)",0)));    
                                break;
                            }
        default:            break;
    }
}

int ItemDialog::exec(Item *item)
{
    // initialization
    QString content, file;
    Item::Type itemType = Item::tBasic;
    Item::State itemState = Item::sNone;
    if (item == NULL)
    {
        setWindowTitle(QApplication::translate("itemDialog", "Create a new item", 0));
        pushBrother->setVisible(true);
        pushChild->setText(QApplication::translate("itemDialog", "C&hild", 0));
    }
    else
    {
        setWindowTitle(QApplication::translate("itemDialog", "Edit the item", 0));
        pushBrother->setVisible(false);
        pushChild->setText(QApplication::translate("itemDialog", "&Edit", 0));
        // pre-filling
        content = item->content().c_str();
        itemType = item->type();
        itemState = item->state();
        if (itemType != Item::tBasic)
        {
            file = dynamic_cast<FileItem*>(item)->fileName().c_str();
        }
    }

    // resets the window
    // state
    radioNone->setChecked(itemState == Item::sNone);
    radioProgress->setChecked(itemState == Item::sProgress);
    radioSuccess->setChecked(itemState == Item::sSuccess);
    radioFailure->setChecked(itemState == Item::sFailure);
    // type
    radioBasic->setChecked(itemType == Item::tBasic);
    radioSound->setChecked(itemType == Item::tSound);
    radioImage->setChecked(itemType == Item::tImage);
    // everuthing else
    toolBrowse->setEnabled(itemType != Item::tBasic);

    editItem->setText(content);
    editItem->setFocus();
    editFile->setText(file);

    // shows it
    return QDialog::exec();
}
