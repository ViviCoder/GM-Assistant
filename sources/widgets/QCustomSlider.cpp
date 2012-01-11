/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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

#include "QCustomSlider.h"

QCustomSlider::QCustomSlider(QWidget *parent): QSlider(parent)
{
}

void QCustomSlider::wheelEvent(QWheelEvent *e)
{
    int delta = e->delta();
    if (delta > 0)
    {
        emit wheeled(true);
    }
    else if (delta < 0)
    {
        emit wheeled(false);
    }
}

void QCustomSlider::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        setValue(e->x()*maximum()/width());
        emit released();
    }
    QSlider::mouseReleaseEvent(e);
}
