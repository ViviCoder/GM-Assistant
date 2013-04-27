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

SelectCharacterDialog::SelectCharacterDialog(QWidget *parent): QDialog(parent), smMapper(new QSignalMapper(this)), pCombat(new CombatDialog(parent))
{
    setupUi(this);
    connect(smMapper, SIGNAL(mapped(QWidget*)), this, SLOT(update(QWidget*)));
}

void SelectCharacterDialog::exec(const CharacterList &list)
{
    iNumber = list.count();
    QFormLayout *formLayout = new QFormLayout();
    checkList = new QCheckBox[iNumber];
    spinList = new QSpinBox[iNumber];
    int i = 0;
    for (CharacterList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        // setting up the check box
        checkList[i].setText(it->name().c_str());
        // setting up the spin box
        spinList[i].setMinimum(1);
        spinList[i].setMaximum(10);
        spinList[i].setValue(1);
        spinList[i].setEnabled(false);
        smMapper->setMapping(checkList+i, spinList+i);
        connect(checkList+i, SIGNAL(clicked()), smMapper, SLOT(map()));
        formLayout->addRow(checkList+i, spinList+i);
        i++;
    }
    groupBox->setLayout(formLayout);
    QDialog::exec();
    delete formLayout;
    delete[] checkList;
    delete[] spinList;
}

void SelectCharacterDialog::update(QWidget *widget)
{
    widget->setEnabled(!widget->isEnabled());
}

void SelectCharacterDialog::accept()
{
    QStringList list;
    for (int i = 0; i < iNumber; i++)
    {
        if (checkList[i].isChecked())
        {
            int n = spinList[i].value();
            if (n == 1)
            {
                list.append(checkList[i].text());
            }
            else
            {
                for (int j = 0; j < spinList[i].value(); j++)
                {
                    list.append(checkList[i].text() + QString(" (%1)").arg(j+1));
                }
            }
        }
    }
    if (list.size() > 1)
    {
        QDialog::accept();
        pCombat->show(list);
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
