/*************************************************************************
* Copyright © 2016-2020 Vincent Prat & Simon Nicolas
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
#include <QAction>
#include <QMenu>
#include "RenameNoteDialog.h"

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
         * \brief Indicator of unregistered modifications
         * \return True if there is an unregistered modification
         */
        bool unregisteredModification() const;
        /*!
         * \brief Overriden method for installing event filter
         * \param filter Filter
         *
         * This method stores the filter instead of actually using it, and installs it for all subwidgets
         */
        void installEventFilter(QObject *filter);
        /*!
         * \brief Method to rename a note
         * \param index Index of the tab
         */
        void renameNote(int index);
        /*!
         * \brief Getter for the note at a given index
         * \param index Index of the tab
         * \return Note at the given index
         */
        inline Note* note(int index);
    public slots:
        /*!
         * \brief Update of the display
         */
        void updateDisplay();
        /*!
         * \brief Update after a modification
         * \param modification Modification to undo or redo
         * \param undo True to undo, false to redo
         */
        void updateModification(NoteModification *modification, bool undo);
        //! Clear the widget
        void clear();
        /*!
         * \brief Open a note
         * \param note Note to open
         * \return TextEdit widget corresponding to the note
         */
        QCustomTextEdit* openNote(Note *note);
        /*!
         * \brief Slot for when a tab is closing
         * \param index Index of the tab to close
         */
        void onTabCloseRequested(int index);
        /*!
         * \brief Delete a note
         * \param note Note to delete
         */
        void deleteNote(Note *note);
        /*!
         * \brief Slot for when a tab is right-clicked
         * \param index Index of the tab
         * \param position Global position of the click
         */
        void onRightClicked(int index, const QPoint& position);
        /*!
         * \brief Slot for when a tool tip is requested
         * \param index Index of the tab
         * \param position Global position of the tool tip
         */
        void onToolTipRequested(int index, const QPoint& position);
    signals:
        /*!
         * \brief Signal emitted when a note is opened
         * \param note Editor for this note
         */
        void noteOpened(QCustomTextEdit *note);
        /*!
         * \brief Signal to register a modification
         * \param modification Modification to register
         */
        void modificationDone(Modification *modification);
    protected:
         /*!
         * \brief Retranslator
         *
         * Retranslates the menu
         */
        void retranslate();
    private:
        //! Map of notes and widgets
        std::map<Note*, QCustomTextEdit*> mNotes;
        //! Event filter
        QObject *pFilter;
        //! Popup menu
        QMenu *pMenu;
        //! Action for renaming
        QAction *actionRename;
        //! Action for closure
        QAction *actionClose;
        //! Dialog window for renaming
        RenameNoteDialog *pRenameDialog;
};

Note* QCustomTabWidget::note(int index)
{
    return static_cast<QCustomTextEdit*>(widget(index))->note();
}

#endif
