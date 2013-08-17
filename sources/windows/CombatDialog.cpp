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

#include "CombatDialog.h"

CombatDialog::CombatDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
    header = tableWidget->verticalHeader();
    header->setMovable(true);
    connect(header, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onCharacterMoved(int, int, int)));
}

void CombatDialog::show(const QStringList &list)
{
    iCharacter = -1;
    tableWidget->setRowCount(0);
    tableWidget->setRowCount(list.size());
    int i = 0;
    for (QStringList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(*it);
        tableWidget->setVerticalHeaderItem(i, item);
        i++;
    }
    pushRemove->setEnabled(tableWidget->rowCount() > 2);
    on_pushNext_clicked();
    QDialog::show();
}

void CombatDialog::on_pushNext_clicked()
{
    iCharacter = (iCharacter + 1) % tableWidget->rowCount();
    label->setText(QApplication::translate("combatDialog", "Current character:", 0) + " <strong>"+tableWidget->verticalHeaderItem(header->logicalIndex(iCharacter))->text()+"</strong>");
    tableWidget->setCurrentCell(header->logicalIndex(iCharacter), 0);
}

void CombatDialog::onCharacterMoved(int, int oldVisualIndex, int newVisualIndex)
{
    if (oldVisualIndex < iCharacter && newVisualIndex >= iCharacter)
    {
        iCharacter--;
    }
    else if (oldVisualIndex > iCharacter && newVisualIndex <= iCharacter)
    {
        iCharacter++;
    }
    tableWidget->setCurrentCell(header->logicalIndex(iCharacter), 0);
}

void CombatDialog::on_pushRemove_clicked()
{
    int n = tableWidget->rowCount();
    if (n > 2)
    {
        tableWidget->removeRow(tableWidget->currentRow());
        if (iCharacter == n-1)
        {
            iCharacter = 0;
        }
        tableWidget->setCurrentCell(header->logicalIndex(iCharacter), 0);
        if (n == 3)
        {
            pushRemove->setEnabled(false);
        }
        label->setText(QApplication::translate("combatDialog", "Current character:", 0) + " <strong>"+tableWidget->verticalHeaderItem(header->logicalIndex(iCharacter))->text()+"</strong>");
    }
}

void CombatDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}
