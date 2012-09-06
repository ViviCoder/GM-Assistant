/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
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

#include "QCustomTextEdit.h"

using namespace std;

QCustomTextEdit::QCustomTextEdit(QWidget *parent): QTextEdit(parent)
{
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void QCustomTextEdit::setNotes(string *text)
{
    pNotes = text;
    updateDisplay();
}

void QCustomTextEdit::updateDisplay()
{
    if (pNotes)
    {
        setText(pNotes->c_str());
    }
}

void QCustomTextEdit::onTextChanged()
{
    if (pNotes)
    {
        *pNotes = toPlainText().toStdString();
    }
}

void QCustomTextEdit::focusInEvent(QFocusEvent *e)
{
    sRef = toPlainText();
    QTextEdit::focusInEvent(e);
}

void QCustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    QString sText = toPlainText();
    if (pNotes && sRef != sText)
    {
        emit modificationDone(new NoteModification(*pNotes, sRef.toStdString(), sText.toStdString()));
    }
    QTextEdit::focusOutEvent(e);
}
