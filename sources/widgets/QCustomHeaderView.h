/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMHEADERVIEW
#define HEADER_QCUSTOMHEADERVIEW

#include <QHeaderView>
#include <QMouseEvent>

class QCustomHeaderView: public QHeaderView
{
    Q_OBJECT

    protected:
        void mousePressEvent(QMouseEvent *e);

    public:
        // constructor
        QCustomHeaderView(Qt::Orientation orientation, QWidget *parent=0);

    signals:
        void rightClicked(int index, const QPoint &position);
};

#endif
