/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMTEXTEDIT
#define HEADER_QCUSTOMTEXTEDIT

#include <QTextEdit>
#include "NoteModification.h"

/*!
 * \brief Custom text widget for notes
 */
class QCustomTextEdit: public QTextEdit
{
    Q_OBJECT

    public:
        /*!
         * \brief Constructor
         * \param parent Parent widget
         */
        QCustomTextEdit(QWidget *parent=0);
        /*!
         * \brief Setter for the underlying notes
         * \param text Text of the notes
         */
        void setNotes(std::string *text); 
    public slots:
        /*!
         * \brief Update of the display
         */
        void updateDisplay();
    protected:
        /*!
         * \brief Change event handler
         * \param e Event to handle
         *
         * Updates the underlying notes
         */
        void changeEvent(QEvent *e);
        /*!
         * \brief FocusIn event handler
         * \param e Event to handle
         *
         * Stores the text to compare when focus goes out
         */
        void focusInEvent(QFocusEvent *e);
        /*!
         * \brief FocusOut event handler
         * \param e Event to handle
         *
         * Checks if the text has been modified and send the corresponding signal if it is the case
         */
        void focusOutEvent(QFocusEvent *e);
    private:
        //! Reference text
        QString sRef;
        //! Underlying notes
        std::string *pNotes;
    signals:
        /*!
         * \brief Signal to register a modification
         * \param modification Modification to register
         *
         * This signal is sent when the notes are modified
         */
        void modificationDone(Modification* modification);
};

#endif
