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
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>

using namespace std;

QCustomTextEdit::QCustomTextEdit(QWidget *parent): QTextEdit(parent), pNotes(0), sStatus(sMove), bDropped(false), bPasted(false), bCut(false)
{
    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

void QCustomTextEdit::setNotes(string *text)
{
    pNotes = text;
    updateDisplay();
}

void QCustomTextEdit::updateDisplay(int position, int length)
{
    if (pNotes)
    {
        QScrollBar *hbar = horizontalScrollBar();
        QScrollBar *vbar = verticalScrollBar();
        int h = hbar->value();
        int v = vbar->value();
        bUpdate = true;
        setText(pNotes->c_str());
        bUpdate = false;
        sStatus = sMove;
        sRef = toPlainText();
        if (position || length )
        {
            hbar->setValue(h);
            vbar->setValue(v);
            QTextCursor cursor = textCursor();
            cursor.setPosition(QString(pNotes->substr(0, position).c_str()).length());
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, QString(pNotes->substr(position, length).c_str()).length());
            setTextCursor(cursor);
        }
    }
}

#include <algorithm>
void QCustomTextEdit::onTextChanged()
{
    if (pNotes)
    {
        QString sText = toPlainText();
        *pNotes = sText.toStdString();
        if (sRef == sText || bDropped || bUpdate)
        {
            bPasted = false;
            bCut = false;
            return;
        }
        int cursorPosition = textCursor().position();
        int l1 = sRef.length(); 
        int l2 = sText.length();
        int l = min(l1, l2);
        int index = 0;
        QString::const_iterator it1 = sRef.begin();
        QString::const_iterator it2 = sText.begin();
        while (it1 != sRef.end() && it2 != sText.end() && *it1 == *it2 && index < cursorPosition)
        {
            index++;
            it1++;
            it2++;
        }
        int rindex = 0;
        QString sRefr = sRef;
        std::reverse(sRefr.begin(), sRefr.end());
        QString sTextr = sText;
        std::reverse(sTextr.begin(), sTextr.end());
        it1 = sRefr.begin();
        it2 = sTextr.begin();
        while (it1 != sRefr.end() && it2 != sTextr.end() && *it1 == *it2 && rindex < l - index)
        {
            rindex++;
            it1++;
            it2++;
        }
        if (index + rindex == l1)
        {
            if (sStatus != sInsertion || bPasted)
            {
                checkModification();
                sStatus = sInsertion;
            }
            // addition
            sModif = sText.mid(index, l2 - rindex - index);
            iIndex = sRef.left(index).toStdString().length();
            // we store the text after the last modification
            sOldModif = sText;
            if (bPasted)
            {
                checkModification();
            }
        }
        else if (index + rindex == l2)
        {
            if (sStatus != sDeletion || bCut)
            {
                checkModification();
                sStatus = sDeletion;
            }
            // deletion
            sModif = sRef.mid(index, l1 - rindex - index);
            iIndex = sRef.left(index).toStdString().length();
            // we store the text after the last modification
            sOldModif = sText;
            if (bCut)
            {
                checkModification();
            }
        }
        else
        {
            if (sStatus != sMove)
            {
                checkModification();
                sStatus = sMove;
                onTextChanged();
            }
            else
            {
                // replacement
                sModif = sText.mid(index, l2 - rindex - index);
                sOldModif = sRef.mid(index, l1 - rindex - index);
                iIndex = sRef.left(index).toStdString().length();
                emit modificationDone(new NoteModification(*pNotes, sOldModif.toStdString(), sModif.toStdString(), iIndex));
                sRef = sText;
            }
        }
        bPasted = false;
        bCut = false;
    }
}

void QCustomTextEdit::checkModification()
{
    if (pNotes)
    {
        switch (sStatus)
        {
            case sInsertion:    emit modificationDone(new NoteModification(*pNotes, Modification::aAddition, sModif.toStdString(), iIndex));
                                break;
            case sDeletion:     emit modificationDone(new NoteModification(*pNotes, Modification::aDeletion, sModif.toStdString(), iIndex));
                                break;
            default:    return;
        }
        sRef = sOldModif;
        sStatus = sMove;
    }
}

void QCustomTextEdit::focusInEvent(QFocusEvent *e)
{
    sRef = toPlainText();
    QTextEdit::focusInEvent(e);
}

void QCustomTextEdit::focusOutEvent(QFocusEvent *e)
{
    if (sStatus != sMove)
    {
        checkModification();
        sStatus = sMove;
    }
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

void QCustomTextEdit::dragEnterEvent(QDragEnterEvent *e)
{
    QTextCursor cursor = textCursor();
    QString sText = toPlainText();
    iIndex = sText.left(cursor.selectionStart()).toStdString().length();
    iLength = sText.left(cursor.selectionEnd()).toStdString().length() - iIndex;
    QTextEdit::dragEnterEvent(e);
}

void QCustomTextEdit::dropEvent(QDropEvent *e)
{
    QString sText = toPlainText();
    iNewIndex = sText.left(cursorForPosition(e->pos()).position()).toStdString().length();
    bDropped = true;
    QTextEdit::dropEvent(e);
    bDropped = false;
    if (e->source()->parent() == this)
    {
        QString sNewText = toPlainText();
        if (pNotes && sText != sNewText)
        {
            emit modificationDone(new NoteModification(*pNotes, iIndex, iNewIndex, iLength));
            sRef = sNewText;
        }
    }
}

void QCustomTextEdit::forceCut()
{
    if (!textCursor().selectedText().isNull())
    {
        bCut = true;
    }
}

void QCustomTextEdit::forcePaste()
{
    if (!QApplication::clipboard()->text().isEmpty())
    {
        bPasted = true;
    }
}

void QCustomTextEdit::updateModification(NoteModification *modification, bool undo)
{
    int position = 0;
    int length = 0;
    switch (modification->action())
    {
        case Modification::aMovement:   length = modification->length();
                                        if (undo)
                                        {
                                            position = modification->index();
                                        }
                                        else
                                        {
                                            position = modification->newIndex() - length;
                                        }
                                        break;
        case Modification::aAddition:   position = modification->index();
                                        if (!undo)
                                        {
                                            length = modification->content().length();
                                        }
                                        break;
        case Modification::aDeletion:   position = modification->index();
                                        if (undo)
                                        {
                                            length = modification->content().length();
                                        }
                                        break;
        case Modification::aEdition:    position = modification->index();
                                        if (undo)
                                        {
                                            length = modification->content().length();
                                        }
                                        else
                                        {
                                            length = modification->newContent().length();
                                        }
                                        break;
    }
    updateDisplay(position, length);
}
