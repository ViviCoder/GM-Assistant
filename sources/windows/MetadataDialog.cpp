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

#include "MetadataDialog.h"

MetadataDialog::MetadataDialog(QWidget *parent): QDialog(parent)
{
    setupUi(this);
}

void MetadataDialog::changeEvent(QEvent *e)
{
    if (e->type() == QEvent::LanguageChange)
    {
        retranslateUi(this);
    }    
}

int MetadataDialog::exec(const Metadata &metadata)
{
    mMetadata = metadata;
    // Updating the widgets
    lineTitle->setText(metadata.title().c_str());
    lineAuthor->setText(metadata.author().c_str());
    // Creation date
    Metadata::Date date = metadata.creationDate();
    QDate qDate(date.year(), date.month(), date.day());
    dateCreation->setDate(qDate);
    textDescription->setText(metadata.description().c_str());
    linePlayers->setText(metadata.players().c_str());
    // Game date
    date = metadata.gameDate();
    qDate = QDate(date.year(), date.month(), date.day());
    dateGame->setDate(qDate);
    
    return QDialog::exec();
}

void MetadataDialog::accept()
{
    mMetadata.setTitle(lineTitle->text().toStdString());
    mMetadata.setAuthor(lineAuthor->text().toStdString());
    QDate qDate = dateCreation->date();
    mMetadata.setCreationDate(Metadata::Date(qDate.day(), qDate.month(), qDate.year()));
    mMetadata.setDescription(textDescription->toPlainText().toStdString());
    mMetadata.setPlayers(linePlayers->text().toStdString());
    qDate = dateGame->date();
    mMetadata.setGameDate(Metadata::Date(qDate.day(), qDate.month(), qDate.year()));
    // parent
    QDialog::accept();
}

Metadata MetadataDialog::metadata() const
{
    return mMetadata;
}
