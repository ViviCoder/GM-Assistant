/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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

#include "QCustomHeaderView.h"

QCustomHeaderView::QCustomHeaderView(Qt::Orientation orientation, QWidget *parent): QHeaderView(orientation,parent)
{
    // enabling drag & drop
    setSectionsMovable(true);
    // enabling auto-resize
    setSectionResizeMode(QHeaderView::ResizeToContents);
    // enabling row or column selection
    setSectionsClickable(true);
}

void QCustomHeaderView::mousePressEvent(QMouseEvent *e)
{
    emit clicked(logicalIndexAt(e->pos()), e->button(), e->globalPos());
    QHeaderView::mousePressEvent(e);
}

bool QCustomHeaderView::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent*>(e);
        emit toolTipRequested(visualIndex(logicalIndexAt(helpEvent->pos())), helpEvent->globalPos());
        return true;
    }
    return QHeaderView::event(e);
}
