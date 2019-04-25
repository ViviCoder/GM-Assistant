/*************************************************************************
* Copyright © 2016-2019 Vincent Prat & Simon Nicolas
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
#include "QCustomTabBar.h"
#include <QToolTip>

using namespace std;

QCustomTabWidget::QCustomTabWidget(QWidget *parent): QTabWidget(parent), pFilter(0)
{
    QTabBar *bar = new QCustomTabBar(this);
    setTabBar(bar);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    connect(bar, SIGNAL(rightClicked(int, const QPoint&)), this, SLOT(onRightClicked(int, const QPoint&)));
    connect(bar, SIGNAL(toolTipRequested(int, const QPoint&)), this, SLOT(onToolTipRequested(int, const QPoint&)));
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
        // create a new TextEdit widget and add it to the tab
        QCustomTextEdit *textEdit = new QCustomTextEdit(this);
        textEdit->setNote(note);
        addTab(textEdit, note->title().c_str());
        note->setVisible(true);
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

void QCustomTabWidget::deleteNote(Note *note)
{
    map<Note*, QCustomTextEdit*>::const_iterator it = mNotes.find(note);
    if (it != mNotes.end())
    {
        delete (*it).second;
        mNotes.erase(it);
    }
}

void QCustomTabWidget::onRightClicked(int index, const QPoint& position)
{
    // TODO
}

void QCustomTabWidget::onToolTipRequested(int index, const QPoint& position)
{
    QToolTip::showText(position, tabText(index));
}
