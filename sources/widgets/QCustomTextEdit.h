/*************************************************************************
* Copyright © 2012-2013 Vincent Prat & Simon Nicolas
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
#include <QKeyEvent>
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
        /*!
         * \brief Modification checking
         *
         * Checks if the text has been modified and send the corresponding signal if it is the case
         */
        void checkModification();
    protected:
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
         * Checks if the text has been modified before releasing focus
         */
        void focusOutEvent(QFocusEvent *e);
        /*!
         * \brief KeyPress event handler
         * \param e Event to handle
         *
         * Decomposes edition in several modifications (insertion, deletion, etc.)
         */
        void keyPressEvent(QKeyEvent *e);
        /*!
         * \brief MousePress event handler
         * \param e Event to handle
         *
         * Works together with keyPressEvent
         */
        void mousePressEvent(QMouseEvent *e);
    protected slots:
        /*!
         * \brief Slot for when the text has changed
         *
         * Updates the underlying notes
         */
        void onTextChanged();
    private:
        //! Edition status
        enum Status
        {
            //! Insertion
            sInsertion,
            //! Deletion
            sDeletion,
            //! Move
            sMove
        };
        //! Reference text
        QString sRef;
        //! Underlying notes
        std::string *pNotes;
        //! Edition status
        Status sStatus; 
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
