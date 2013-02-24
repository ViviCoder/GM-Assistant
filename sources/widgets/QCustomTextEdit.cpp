/*************************************************************************
* Copyright © 2012-2013 Vincent Prat & Simon Nicolas
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

QCustomTextEdit::QCustomTextEdit(QWidget *parent): QTextEdit(parent), pNotes(0), sStatus(sInsertion)
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
        sRef = toPlainText();
    }
}

void QCustomTextEdit::onTextChanged()
{
    if (pNotes)
    {
        *pNotes = toPlainText().toStdString();
    }
}

void QCustomTextEdit::checkModification()
{
    QString sText = toPlainText();
    if (pNotes && sRef != sText)
    {
        emit modificationDone(new NoteModification(*pNotes, sRef.toStdString(), sText.toStdString()));
        sRef = sText;
    }
}

void QCustomTextEdit::focusInEvent(QFocusEvent *e)
{
    sRef = toPlainText();
    QTextEdit::focusInEvent(e);
}

void QCustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    checkModification();
    QTextEdit::focusOutEvent(e);
}

void QCustomTextEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_Left:
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:  if (sStatus != sMove)
                                {
                                    checkModification();
                                    sStatus = sMove;
                                }
                                break;
        case Qt::Key_Delete:
        case Qt::Key_Backspace: if (sStatus != sDeletion)
                                {
                                    checkModification();
                                    sStatus = sDeletion;
                                }
                                break;
        default:    if (sStatus != sInsertion)
                    {
                        checkModification();
                        sStatus = sInsertion;
                    }
    }
    QTextEdit::keyPressEvent(e);
}

void QCustomTextEdit::mousePressEvent(QMouseEvent *e)
{
    if (sStatus != sMove)
    {
        checkModification();
        sStatus = sMove;
    }
    QTextEdit::mousePressEvent(e);
}
