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
#include <QCheckBox>
#include <QSpinBox>

SelectCharacterDialog::SelectCharacterDialog(QWidget *parent): QDialog(parent), smMapper(new QSignalMapper(this))
{
    setupUi(this);
    connect(smMapper, SIGNAL(mapped(QWidget*)), this, SLOT(update(QWidget*)));
}

void SelectCharacterDialog::exec(const CharacterList &list)
{
    int n = list.count();
    QFormLayout *formLayout = new QFormLayout();
    QCheckBox *checkList = new QCheckBox[n];
    QSpinBox *spinList = new QSpinBox[n];
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
