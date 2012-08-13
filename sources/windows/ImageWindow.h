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
 * \brief Window used to display an image
 */
class ImageWindow: public QLabel
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
        /*!
         * Aspect ratio of the image
         */
        double dAspectRatio;
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
    public:
        /*!
         * \brief Constructor of the window
         * \param imageFileName Name of the file containing the image to show
         * \param parent Parent widget
         */
        ImageWindow(const std::string &imageFileName, QWidget *parent=0);
        /*!
         * \brief Getter for the error flag
         * \return True if the image was not loaded correctly
         */
        bool error() const;
};

#endif
