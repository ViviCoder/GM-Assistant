/*************************************************************************
* Copyright © 2019-2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMTABBAR
#define HEADER_QCUSTOMTABBAR

#include <QTabBar>
#include <QMouseEvent>

//! Bar for the custom tab widget
class QCustomTabBar: public QTabBar
{
    Q_OBJECT

    protected:
        /*!
         * \brief MousePress event handler
         * \param e Event to handle
         *
         * Detect right clicks and send the rightClicked signal
         */
        void mousePressEvent(QMouseEvent *e) override;
        /*!
         * \brief Event handler
         * \param e Event
         *
         * This handler is used to display dynamical tool tips.
         */
        bool event(QEvent *e) override;

    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        QCustomTabBar(QWidget *parent=0);

    signals:
        /*!
         * \brief Signal emitted when the bar is right-clicked
         * \param index Index of the clicked tab
         * \param position Global position of the click (used for showing a popup menu)
         */
        void rightClicked(int index, const QPoint &position);
        /*!
         * \brief Signal emitted when a tool tip should be displayed
         * \param index Index of the tab
         * \param position Global position of the tool tip
         */
        void toolTipRequested(int index, const QPoint &position);
};

#endif
