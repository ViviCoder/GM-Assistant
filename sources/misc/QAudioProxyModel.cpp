/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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

QAudioProxyModel::QAudioProxyModel(QWidget *parent, const QString &installDir): QSortFilterProxyModel(parent), cookie(magic_open(MAGIC_MIME_TYPE)), slFormats(Phonon::BackendCapabilities::availableMimeTypes()), bFiltering(true)
{
    magic_load(cookie, 0);
    if (magic_error(cookie))
    {
        magic_close(cookie);
        cookie = magic_open(MAGIC_MIME_TYPE);
        magic_load(cookie, (installDir + "magic.mgc").toStdString().c_str());
    }
    if (magic_error(cookie))
    {
        bFiltering = false;
    }
    slFormats = slFormats.filter("audio/");
}

QAudioProxyModel::~QAudioProxyModel()
{
    magic_close(cookie);
}

bool QAudioProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!bFiltering)
    {
        return true;
    }   
    QFileSystemModel *model = dynamic_cast<QFileSystemModel*>(sourceModel());
    QModelIndex index = model->index(sourceRow, 0, sourceParent);
    if (model->isDir(index))
    {
        return true;
    }
    return slFormats.contains(magic_file(cookie, model->filePath(index).toStdString().c_str()));
}
