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

#include "DiceDialog.h"
#include <ctime>

DiceDialog::DiceDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
    /* Initializes the random seed */
    srand(time(0));
}

void DiceDialog::on_pushThrow_clicked()
{
    int faces;
    switch (comboType->currentIndex())
    {
        case 0:     faces = 2;
                    break;
        case 1:     faces = 3;
                    break;
        case 2:     faces = 4;
                    break;
        case 3:     faces = 6;
                    break;
        case 4:     faces = 8;
                    break;
        case 5:     faces = 10;
                    break;
        case 6:     faces = 12;
                    break;
        case 7:     faces = 20;
                    break;
        default:    faces = 100;
    }
    QString result;
    int nb = spinNumber->value();
    for (int i = 0; i < nb; i++)
    {
        result += QString("%1").arg(rand() % faces + 1);
        if (i+1 < nb)
        {
            result += " ";
        }
    }
    textResult->setPlainText(result);
}

void DiceDialog::on_pushReset_clicked()
{
    textResult->clear();
}

void DiceDialog::show()
{
    on_pushReset_clicked();
    QDialog::show();
}

void DiceDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}
