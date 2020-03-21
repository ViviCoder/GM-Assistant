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
    // limiting the possible values for counting
    spinEqual->setMaximum(faces);
    spinLeast->setMaximum(faces);
    spinMost->setMaximum(faces);

    int nb = spinNumber->value();

    // results
    vResults.resize(nb);
    QString result;
    int sum = 0;
    int min = faces;
    int max = 1;
    int nequal = 0;
    int nleast = 0;
    int nmost = 0;

    for (int i = 0; i < nb; i++)
    {
        int res = rand() % faces + 1;
        vResults[i] = res;
        result += QString("%1").arg(res);
        if (i+1 < nb)
        {
            result += " ";
        }
        // computations
        sum += res;
        if (res > max)
        {
            max = res;
        }
        if (res < min)
        {
            min = res;
        }
        if (res == spinEqual->value())
        {
            nequal++;
        }
        if (res >= spinLeast->value())
        {
            nleast++;
        }
        if (res <= spinMost->value())
        {
            nmost++;
        }
    }
    textResult->setPlainText(result);
    lineSum->setText(QString("%1").arg(sum));
    lineMin->setText(QString("%1").arg(min));
    lineMax->setText(QString("%1").arg(max));
    lineEqual->setText(QString("%1").arg(nequal));
    lineLeast->setText(QString("%1").arg(nleast));
    lineMost->setText(QString("%1").arg(nmost));
}

void DiceDialog::on_pushReset_clicked()
{
    vResults.clear();
    textResult->clear();
    lineSum->clear();
    lineMin->clear();
    lineMax->clear();
    lineEqual->clear();
    lineLeast->clear();
    lineMost->clear();
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

void DiceDialog::on_spinEqual_valueChanged(int value)
{
    int count = 0;
    for (std::vector<int>::const_iterator it = vResults.begin(); it != vResults.end(); it++)
    {
        if (*it == value)
        {
            count++;
        }
    }
    if (vResults.size() > 0)
    {
        lineEqual->setText(QString("%1").arg(count));
    }
}

void DiceDialog::on_spinLeast_valueChanged(int value)
{
    int count = 0;
    for (std::vector<int>::const_iterator it = vResults.begin(); it != vResults.end(); it++)
    {
        if (*it >= value)
        {
            count++;
        }
    }
    if (vResults.size() > 0)
    {
        lineLeast->setText(QString("%1").arg(count));
    }
}

void DiceDialog::on_spinMost_valueChanged(int value)
{
    int count = 0;
    for (std::vector<int>::const_iterator it = vResults.begin(); it != vResults.end(); it++)
    {
        if (*it <= value)
        {
            count++;
        }
    }
    if (vResults.size() > 0)
    {
        lineMost->setText(QString("%1").arg(count));
    }
}
