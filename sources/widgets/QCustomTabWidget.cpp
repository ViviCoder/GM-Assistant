/*************************************************************************
* Copyright © 2016 Vincent Prat & Simon Nicolas
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

#include "QCustomTabWidget.h"

using namespace std;

QCustomTabWidget::QCustomTabWidget(QWidget *parent): QTabWidget(parent)
{
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void QCustomTabWidget::setNotes(const vector<Note*> &notes)
{
    clear();
    for (vector<Note*>::const_iterator it = notes.begin(); it != notes.end(); it++)
    {
        if ((*it)->visible())
        {
            QCustomTextEdit *widget = new QCustomTextEdit(this);
            addTab(widget, (*it)->title().c_str());
        }
    } 
    updateDisplay();
}

void QCustomTabWidget::clear()
{
    // TODO
}

void QCustomTabWidget::updateDisplay()
{
    // TODO
}

bool QCustomTabWidget::unregisteredModification() const
{
    // TODO
    return false;
}

void QCustomTabWidget::checkModification()
{
    // TODO
}

void QCustomTabWidget::updateModification(NoteModification*, bool)
{
    // TODO
}

void QCustomTabWidget::forceCut()
{
    // TODO
}

void QCustomTabWidget::forcePaste()
{
    // TODO
}
