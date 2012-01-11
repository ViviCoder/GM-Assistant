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

#ifndef HEADER_QCUSTOMSLIDER
#define HEADER_QCUSTOMSLIDER

#include <QSlider>
#include <QMouseEvent>

/*!
 * \brief Custom slider
 *
 * Implements custom mouse (click) and wheel events
 */
class QCustomSlider: public QSlider
{
    Q_OBJECT

    protected:
        /*!
         * \brief Event raised when the wheel is used
         *
         * Sends a corresponding signal
         */
        void wheelEvent(QWheelEvent *e);

    public:
        /*!
         * \brief Constructor
         * \param parent Parent of the slider
         */
        QCustomSlider(QWidget *parent=NULL);

    signals:
        /*!
         * \brief Signal sent when wheeled
         * \param positive True if wheeled forward, false otherwise
         */
        void wheeled(bool positive);
};

#endif
