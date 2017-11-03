/*************************************************************************
* Copyright © 2013-2017 Vincent Prat & Simon Nicolas
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
#include <QMessageBox>

CombatDialog::CombatDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
    header = tableWidget->verticalHeader();
    header->setSectionsMovable(true);
    header->setSectionResizeMode(QHeaderView::Fixed);
    connect(header, SIGNAL(sectionMoved(int, int, int)), this, SLOT(onCharacterMoved(int, int, int)));
    connect(header, SIGNAL(sectionDoubleClicked(int)), this, SLOT(onCharacterSelected(int)));
}

void CombatDialog::show(const QStringList &list)
{
    iCharacter = 0;
    tableWidget->setRowCount(0);
    tableWidget->setRowCount(list.size());
    int i = 0;
    for (QStringList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        QTableWidgetItem *headerItem = new QTableWidgetItem(*it);
        QTableWidgetItem *item = new QTableWidgetItem("");
        tableWidget->setVerticalHeaderItem(i, headerItem);
        tableWidget->setItem(i, 0, item);
        i++;
    }
    pushRemove->setEnabled(tableWidget->rowCount() > 2);
    updateDisplay();
    QDialog::show();
}

void CombatDialog::on_pushNext_clicked()
{
    iCharacter = (iCharacter + 1) % tableWidget->rowCount();
    updateDisplay();
}

void CombatDialog::updateDisplay()
{
    int logical = header->logicalIndex(iCharacter);
    for (int n = 0; n < tableWidget->rowCount(); n++)
    {
        if (n == logical)
        {
            tableWidget->item(n, 0)->setBackground(QPalette().color(QPalette::ToolTipBase));
        }
        else
        {
            tableWidget->item(n, 0)->setBackground(QPalette().color(QPalette::Base));
        }
    }
    label->setText(QApplication::translate("combatDialog", "Current character:", 0) + " <strong>"+tableWidget->verticalHeaderItem(logical)->text()+"</strong>");
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
    else if (oldVisualIndex == iCharacter)
    {
        if (newVisualIndex < iCharacter)
        {
            on_pushNext_clicked();
        }
        else
        {
            updateDisplay();
        }
    }
}

void CombatDialog::on_pushRemove_clicked()
{
    int n = tableWidget->rowCount();
    if (n > 2)
    {
        int row = tableWidget->currentRow();
        if (QMessageBox::question(this, QApplication::translate("combatDialog", "Confirmation", 0), QApplication::translate("combatDialog", "You are about to remove %1 from the combat manager. Are you sure you want to do it?", 0).arg("<strong>"+tableWidget->verticalHeaderItem(row)->text()+"</strong>"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
        {
            tableWidget->removeRow(row);
            if (iCharacter > row)
            {
                iCharacter--;
            }
            if (n == 3)
            {
                pushRemove->setEnabled(false);
            }
            updateDisplay();
        }
    }
}

void CombatDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}

void CombatDialog::onCharacterSelected(int logicalIndex)
{
    iCharacter = header->visualIndex(logicalIndex);
    updateDisplay();
}

void CombatDialog::closeEvent(QCloseEvent *e)
{
    if (QMessageBox::question(this, QApplication::translate("combatDialog", "Confirmation", 0), QApplication::translate("combatDialog", "You are about to close the combat manager. You will lose all information about the ongoing combat. Are you sure you want to do it?", 0), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void CombatDialog::on_pushClose_clicked()
{
    close();
}
