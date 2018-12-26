/*************************************************************************
* Copyright © 2016-2018 Vincent Prat & Simon Nicolas
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

QCustomTabWidget::QCustomTabWidget(QWidget *parent): QTabWidget(parent), pFilter(0)
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
}

void QCustomTabWidget::setNotes(const vector<Note*> &notes)
{
    clear();
    for (vector<Note*>::const_iterator it = notes.begin(); it != notes.end(); it++)
    {
        if ((*it)->visible())
        {
            QCustomTextEdit *widget = new QCustomTextEdit(this);
            if (pFilter)
            {
                widget->installEventFilter(pFilter);
            }
            mNotes[*it] = widget;
            widget->setNote(*it);
            emit noteOpened(widget);
            addTab(widget, (*it)->title().c_str());
        }
    } 
    updateDisplay();
}

void QCustomTabWidget::clear()
{
    for (int i = 0; i < count(); i++)
    {
        delete widget(i);
    }
    QTabWidget::clear();
    mNotes.clear();
}

void QCustomTabWidget::updateDisplay()
{
    // TODO
}

bool QCustomTabWidget::unregisteredModification() const
{
    return dynamic_cast<QCustomTextEdit*>(currentWidget())->unregisteredModification();
}

void QCustomTabWidget::updateModification(NoteModification *modification, bool undo)
{
    map<Note*, QCustomTextEdit*>::const_iterator it = mNotes.find(&modification->note());
    if (it != mNotes.end())
    {
        it->second->updateModification(modification, undo);
    }
    // TODO add it if not found + set focus
}

void QCustomTabWidget::forceCut()
{
    // TODO
}

void QCustomTabWidget::forcePaste()
{
    // TODO
}

void QCustomTabWidget::installEventFilter(QObject *filter)
{
    pFilter = filter;
}

void QCustomTabWidget::openNote(Note *note)
{
    map<Note*, QCustomTextEdit*>::const_iterator it = mNotes.find(note);
    if (it == mNotes.end())
    {
        QCustomTextEdit *textEdit = new QCustomTextEdit(this);
        addTab(textEdit, note->title().c_str());
        mNotes.insert(pair<Note*, QCustomTextEdit*>(note, textEdit));
        emit noteOpened(textEdit);
    }
    else if(note->visible())
    {
        // TODO: focus if found
    }
    else
    {
        // reopen the note if previously closed
        addTab((*it).second, note->title().c_str());
        note->setVisible(true);
    }
}

void QCustomTabWidget::onTabCloseRequested(int index)
{
    if (index > 0)
    {
        QCustomTextEdit *textEdit = dynamic_cast<QCustomTextEdit*>(widget(index));
        textEdit->note()->setVisible(false);
        removeTab(index);
    }
}
