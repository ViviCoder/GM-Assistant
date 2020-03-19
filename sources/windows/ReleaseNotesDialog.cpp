/*************************************************************************
* Copyright © 2020 Vincent Prat & Simon Nicolas
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

#include "ReleaseNotesDialog.h"
#include <Poco/FileStream.h>

using namespace std;

ReleaseNotesDialog::ReleaseNotesDialog(QWidget *parent, const QString &install_dir): QDialog(parent)
{
    setupUi(this);

    // loading the file
    QString fileName(install_dir + "CHANGELOG");
    string line;
    string text;
    try
    {
        Poco::FileInputStream ibuf(fileName.toStdString());
        while (getline(ibuf, line))
        {
            text += line + "\n";
        }
        textEdit->setText(text.c_str());
    }
    catch (exception &e)
    {
        textEdit->setText(e.what());
    }
}

void ReleaseNotesDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }
}
