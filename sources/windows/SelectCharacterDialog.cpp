/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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

SelectCharacterDialog::SelectCharacterDialog(QWidget *parent): QDialog(parent), pCombat(new CombatDialog(parent))
{
    setupUi(this);
}

void SelectCharacterDialog::show(const CharacterList &list)
{
    listAll->clear();
    for (CharacterList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        listAll->addItem((*it).name().c_str());
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
        listInvolved->addItem(item->text());
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
