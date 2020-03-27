/*************************************************************************
* Copyright © 2013-2020 Vincent Prat & Simon Nicolas
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

#include "SelectCharacterDialog.h"
#include <QFormLayout>
#include <QMessageBox>

SelectCharacterDialog::SelectCharacterDialog(QWidget *parent, CombatDialog *combatDialog): QDialog(parent), pCombat(combatDialog)
{
    setupUi(this);
}

void SelectCharacterDialog::show(const CharacterList &list)
{
    // ask confirmation before closing the combat manager if already open
    if (pCombat->isVisible())
    {
        if (QMessageBox::question(this, QApplication::translate("selectCharacterDialog", "Confirmation", 0), QApplication::translate("selectCharacterDialog", "You are about to reset the combat manager. You will lose all information about the ongoing combat. Are you sure you want to do it?", 0), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
        {
            reject();
            return;
        }
        else
        {
            pCombat->setVisible(false);
        }
    }
    // initialisation
    listAll->clear();
    for (CharacterList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        listAll->addItem((*it)->name().c_str());
    }
    listInvolved->clear();
    QDialog::show();
}

void SelectCharacterDialog::accept()
{
    int n = listInvolved->count();
    if (n > 1)
    {
        QStringList list;
        for (int i = 0; i < n; i++)
        {
            list.append(listInvolved->item(i)->text());
        }
        pCombat->show(list);
        QDialog::accept();
    }
    else
    {
        QMessageBox::critical(this, QApplication::translate("selectCharacterDialog", "Uncompleted character selection", 0), QApplication::translate("selectCharacterDialog", "You have selected too few characters. There must be at least two of them.", 0));
    }
}

void SelectCharacterDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}

void SelectCharacterDialog::on_pushAdd_clicked()
{
    QListWidgetItem *item = listAll->currentItem();
    if (item)
    {
        int position = listInvolved->currentRow();
        if (position < 0)
        {
            listInvolved->addItem(item->text());
        }
        else
        {
            // if an item is selected in the list of involved characters, insert the new one just after and select it
            listInvolved->insertItem(position+1, item->text());
            listInvolved->setCurrentRow(position+1);
        }
    }
}

void SelectCharacterDialog::on_pushRemove_clicked()
{
    QListWidgetItem *item = listInvolved->currentItem();
    if (item)
    {
        listInvolved->removeItemWidget(item);
        delete item;
    }
}

void SelectCharacterDialog::on_pushUp_clicked()
{
    int n = listInvolved->currentRow();
    if (n > 0)
    {
        QListWidgetItem *item = listInvolved->takeItem(n);
        listInvolved->insertItem(n - 1, item);
        listInvolved->setCurrentRow(n - 1);
    }
}

void SelectCharacterDialog::on_pushDown_clicked()
{
    int n = listInvolved->currentRow();
    if (n >= 0 && n < listInvolved->count()-1)
    {
        QListWidgetItem *item = listInvolved->takeItem(n);
        listInvolved->insertItem(n + 1, item);
        listInvolved->setCurrentRow(n + 1);
    }
}
