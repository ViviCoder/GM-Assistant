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

#include "PictureWindow.h"

PictureWindow::PictureWindow(std::string pictureFileName):pictureToShow(new QPixmap)//: QWidget(parent)
{
    setupUi(this);
    //QPixmap *pictureToShow = new QPixmap(pictureFileName.c_str());
    pictureToShow->load(pictureFileName.c_str());
    QLabelPicture->setPixmap(*pictureToShow);
    //QLabelPicture->setPixmap(QPixmap(pictureFileName.c_str()));
    showNormal();
    //setWindowTitle((pictureItem->content()).c_str());

}

void PictureWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (!isFullScreen())
    {
        showFullScreen();
        QLabelPicture->setScaledContents(true);
    }
    else
    {    
        showNormal();
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
