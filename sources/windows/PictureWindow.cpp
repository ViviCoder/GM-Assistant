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
#include <QPainter>

PictureWindow::PictureWindow(const std::string &pictureFileName, QWidget *parent): QLabel(parent), bError(false), sImageFileName(pictureFileName), bSvg(true), renderer(NULL) 
{
    setAlignment(Qt::AlignCenter);
    setWindowFlags(windowFlags()|Qt::Window);
    // setting backgournd color to black
    QPalette newPalette(palette());
    newPalette.setColor(QPalette::Window,QColor("black"));
    setPalette(newPalette);
    // displaying the picture
    if (sImageFileName.substr(sImageFileName.length()-3,3) != "svg")
    {
        bSvg = false;
        QPixmap pix(sImageFileName.c_str());
        if (pix.isNull())
        {
            // the image cannot be loaded
            bError = true;
            sImageFileName = ":/data/images/stop.svg";
            bSvg = true;
        }
        else
        {
            setPixmap(pix);
        }
    }
    if (bSvg)
    {
        renderer = new QSvgRenderer(this);
        if (!renderer->load(QString(sImageFileName.c_str())))
        {
            // unable to load the image
            renderer->load(QString(":/data/images/stop.svg"));
            bError = true;
        }
        resize(renderer->defaultSize());
        // the size is fixed when the image does not exist
        if (bError)
        {
            setFixedSize(size());
        }
    }
    showNormal();
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
    if (pixmap() != NULL)
    {
        setPixmap(pixmap()->scaled(e->size(),Qt::KeepAspectRatio));
    }
}

void PictureWindow::paintEvent(QPaintEvent *e)
{
    // special treatment for svg images
    if (bSvg)
    {
        QPainter painter(this);
        renderer->render(&painter);
    }
    else
    {
        QLabel::paintEvent(e);
    }
}
