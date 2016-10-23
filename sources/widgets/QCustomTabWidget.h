/*************************************************************************
* Copyright © 2016 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMTABWIDGET
#define HEADER_QCUSTOMTABWIDGET

#include <QTabWidget>
#include "QCustomTextEdit.h"
#include "Note.h"

/*!
 * \brief Custom tab widget for notes
 */
class QCustomTabWidget: public QTabWidget
{
    Q_OBJECT

    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        QCustomTabWidget(QWidget *parent=0);
        /*!
         * \brief Setter for the underlying notes
         * \param notes Notes
         */
        void setNotes(const std::vector<Note*> &notes); 
        /*!
         * \brief Indicator of unregistered modifications
         * \return True if there is an unregistered modification
         */
        bool unregisteredModification() const;
    public slots:
        /*!
         * \brief Update of the display
         */
        void updateDisplay();
        /*!
         * \brief Modification checking
         *
         * Checks if the text has been modified and send the corresponding signal if it is the case
         */
        void checkModification();
        /*!
         * \brief Update after a modification
         * \param modification Modification to undo or redo
         * \param undo True to undo, false to redo
         */
        void updateModification(NoteModification *modification, bool undo);
        /*!
         * \brief Modification forcing for cuts
         *
         * Forces the widget to register the next deletion as a separate modification
         */
        void forceCut();
        /*!
         * \brief Modification forcing for pastings
         *
         * Forces the widget to register the next insertion as a separate modification
         */
        void forcePaste();

        //! Clear the widget
        void clear();
    signals:
        /*!
         * \brief Signal emitted when a note is opened
         * \param note Editor for this note
         */
        void noteOpened(QCustomTextEdit *note);
    private:
        //! Map of notes and widgets
        std::map<Note*, QCustomTextEdit*> mNotes;
};

#endif
