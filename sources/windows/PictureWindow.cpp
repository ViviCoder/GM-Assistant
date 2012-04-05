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

#include "PictureWindow.h"

PictureWindow::PictureWindow(const std::string &pictureFileName, QWidget *parent): QLabel(parent), bError(false)
{
    setAlignment(Qt::AlignCenter);
    setWindowFlags(windowFlags()|Qt::Window);
    // setting backgournd color to black
    QPalette newPalette(palette());
    newPalette.setColor(QPalette::Window,QColor("black"));
    setPalette(newPalette);
    // displaying the picture
    QPixmap pix(pictureFileName.c_str());
    if (pix.isNull())
    {
        // the image cannot be loaded
        bError = true;
        setPixmap(QPixmap(":/data/images/stop.svg"));
    }
    else
    {
        setPixmap(pix);
    }
    showNormal();
    if (bError)
    {
        setFixedSize(size());
    }
}

void PictureWindow::mouseReleaseEvent(QMouseEvent *)
{
    if (bError)
    {
        close();
        deleteLater();
    }
    else
    {
        if (!isFullScreen())
        {
            showFullScreen();
        }
        else
        {    
            showNormal();
        }
    }
}

void PictureWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Escape:    showNormal();
                                break;
        default: break;
    }
}

void PictureWindow::resizeEvent(QResizeEvent *e)
{
    // overriden event handler 
    QLabel::resizeEvent(e);
    // replacing the pixmap
    setPixmap(pixmap()->scaled(e->size(),Qt::KeepAspectRatio));
}
