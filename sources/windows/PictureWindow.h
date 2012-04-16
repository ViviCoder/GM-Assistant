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

#ifndef HEADER_PICTUREWINDOW
#define HEADER_PICTUREWINDOW

#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <string>
#include <QSvgRenderer>

/*!
 * \brief Window used to display a picture
 */
class PictureWindow: public QLabel
{
    private:
        /*!
         * Flag active if the image cannot be loaded
         */
        bool bError;
        /*!
         * Image file name
         */
        std::string sImageFileName;
        /*!
         * Flag active if the image is in the SVG format
         */
        bool bSvg;
        /*!
         * Svg Renderer
         */
        QSvgRenderer *renderer;
    protected:
        /*!
         * \brief Mouse ReleaseEvent handler
         * \param e Mouse event raised
         *
         * Switches between full screen and normal
         */
        void mouseReleaseEvent(QMouseEvent *e);
        /*!
         * \brief Key ReleaseEvent handler
         * \param e Key event raised
         */
        void keyReleaseEvent(QKeyEvent *e);
        /*!
         * \brief Resize event handler
         * \param e Resize event raised
         */
        void resizeEvent(QResizeEvent *e);
        /*!
         * \brief Paint event handler
         * \param e Paint event raised
         */
        void paintEvent(QPaintEvent *e);
    public:
        /*!
         * \brief Constructor of the window
         * \param pictureFileName Name of the file containing the picture to show
         * \param parent Parent widget
         */
        PictureWindow(const std::string &pictureFileName, QWidget *parent=NULL);
};

#endif
