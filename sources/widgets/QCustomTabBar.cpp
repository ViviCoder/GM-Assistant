/*************************************************************************
* Copyright © 2019 Vincent Prat & Simon Nicolas
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

#include "QCustomTabBar.h"

QCustomTabBar::QCustomTabBar(QWidget *parent): QTabBar(parent)
{
}

void QCustomTabBar::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::RightButton)
    {
        emit rightClicked(tabAt(e->pos()), e->globalPos());
    }
    QTabBar::mousePressEvent(e);
}

bool QCustomTabBar::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = dynamic_cast<QHelpEvent*>(e);
        emit toolTipRequested(tabAt(helpEvent->pos()), helpEvent->globalPos());
        return true;
    }
    return QTabBar::event(e);
}
