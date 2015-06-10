/*************************************************************************
* Copyright © 2013-2015 Vincent Prat & Simon Nicolas
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

#include "QAudioProxyModel.h"
#include <QFileSystemModel>

using namespace Phonon;

QAudioProxyModel::QAudioProxyModel(QWidget *parent, const FileDetector *detector): QSortFilterProxyModel(parent), pDetector(0), slFormats(Phonon::BackendCapabilities::availableMimeTypes().filter("audio/"))
{
    if (detector && detector->isValid())
    {
        pDetector = detector;
    }
}

bool QAudioProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!pDetector)
    {
        return true;
    }   
    QFileSystemModel *model = dynamic_cast<QFileSystemModel*>(sourceModel());
    QModelIndex index = model->index(sourceRow, 0, sourceParent);
    if (model->isDir(index))
    {
        return true;
    }
    QString fileName = model->filePath(index);
    std::string mimeType = pDetector->typeOfFile(fileName.toStdString());
    // special case when mp3 files are not properly recognized as such
    if (fileName.endsWith("mp3") && mimeType == "application/octet-stream")
    {
        return true;
    }
    return slFormats.contains(mimeType.c_str());
}
