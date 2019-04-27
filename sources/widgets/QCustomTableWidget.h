/*************************************************************************
* Copyright © 2011-2019 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMTABLEWIDGET
#define HEADER_QCUSTOMTABLEWIDGET

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMouseEvent>
#include <QMenu>
#include "ChangePropertyDialog.h"
#include "ChangeCharacterDialog.h"
#include "PropertyList.h"
#include "CharacterList.h"
#include "CharacterModification.h"
#include "QCustomHeaderView.h"

/*!
 * \brief Custom table widget
 *
 * This widget is designed to manipulate underlying CharacterList and PropertyList
 */
class QCustomTableWidget: public QTableWidget
{
    Q_OBJECT

    private:
        //! Main menu
        QMenu *menu;
        //! Menu of the horizontal header (properties)
        QMenu *hMenu;
        //! Menu of the vertical header (characters)
        QMenu *vMenu;
        //! Action for column addition
        QAction *actionAddColumn;
        //! Action for column deletion
        QAction *actionRemoveColumn;
        //! Action for row addition
        QAction *actionAddRow;
        //! Action for row deletion
        QAction *actionRemoveRow;
        //! Action for row edition
        QAction *actionEditRow;
        //! Action for column edition
        QAction *actionEditColumn;
        ChangePropertyDialog *pChangePropertyDial;
        ChangeCharacterDialog *pChangeCharacterDial;
        //! Underlying list of properties
        PropertyList *pProperties;
        //! Underlying list of characters
        CharacterList *pCharacters;
        //! Flag indicating if an item is currently being edited
        bool bEditing;
        //! Flag indicating if the widget is currently updating
        bool bUpdate;
        //! Number of recently created cells
        int iCreatedCells;
        //! Vertical header
        QCustomHeaderView *pVHeader;
        //! Type of selection
        QAbstractItemView::SelectionBehavior eSelected;
    protected:
        /*!
         * \brief Event raised when the table is clicked
         * \param e Mouse event
         */
        void mousePressEvent(QMouseEvent *e);
        /*!
         * \brief KeyReleaseEvent handler
         * \param e Key event
         */
        void keyReleaseEvent(QKeyEvent *e);
        /*!
         * \brief DoubleClickEvent handler
         * \param e Mouse event
         */
        void mouseDoubleClickEvent(QMouseEvent *e);
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         */
        void changeEvent(QEvent *e);
        /*!
         * \brief Addition of a property
         * \param index Index where to add the property
         */
        void addProperty(int index);
        /*!
         * \brief Addition of a character
         * \param index Index where to add the character
         */
        void addCharacter(int index);
        /*!
         * \brief Removal of a property
         * \param index Index of the property to be removed
         */
        void removeProperty(int index);
        /*!
         * \brief Removal of a character
         * \param index Index of the character to be removed
         */
        void removeCharacter(int index);
        /*!
         * \brief Edition of a property
         * \param index Index of the property to be edited
         */
        void editProperty(int index);
        /*!
         * \brief Edition of a character
         * \param index Index of the character to be edited
         */
        void editCharacter(int index);
        /*!
         * \brief Retranslator
         *
         * Retranslates the menus
         */
        void retranslate();
        /*!
         * \brief Formater for the horizontal header
         * \param name Character's name
         * \param description Short description
         * \return Formated header text
         */
        static QString headerText(const QString &name, const QString &description);
        /*!
         * \brief Size hint for columns taking into account unvisible ones
         * \param column Column to resize
         */
        int sizeHintForColumn(int column) const;
    protected slots:
        /*!
         * \brief Slot for when a cell changes 
         * \param logicalRow Logical row of the cell
         * \param logicalColumn Logical column of the cell
         */
        void onCellChanged(int logicalRow, int logicalColumn);
        /*!
         * \brief Slot for when the horizontal header is clicked
         * \param index Index of the clicked column
         * \param button Mouse button used to click
         * \param position Global position of the click for popups
         */
        void onHHeaderClicked(int index, Qt::MouseButton button, const QPoint &position);
        /*!
         * \brief Slot for when the vertical header is clicked
         * \param index Index of the clicked row
         * \param button Mouse button used to click
         * \param position Global position of the click for popups
         */
        void onVHeaderClicked(int index, Qt::MouseButton button, const QPoint &position);
        /*!
         * \brief Slot for when a section of the horizontal header is moved
         * \param oldColumn Old visual column
         * \param newColumn New visual column
         */
        void onHHeaderMoved(int, int oldColumn, int newColumn);
        /*!
         * \brief Slot for when a section of the vertical header is moved
         * \param oldRow Old visual row
         * \param newRow New visual row
         */
        void onVHeaderMoved(int, int oldRow, int newRow);
        /*!
         * \brief Slot for when a character is double-clicked
         * \param index Index of the clicked row
         */
        void onCharacterDoubleClicked(int index);
        /*!
         * \brief Slot for when the selection changes 
         */
        void on_itemSelectionChanged();
        /*!
         * \brief Slot for when the vertical header requests a tool tip
         * \param index Index of the row
         * \param position Global position of the event
         */
        void onVHeaderToolTipRequested(int index, const QPoint &position);
    public:
        /*!
         * \brief Constructor of the widget
         * \param parent Parent widget
         */
        QCustomTableWidget(QWidget *parent=0);
        //! destructor
        ~QCustomTableWidget();
        /*!
         * \brief Setter for the underlying property and character lists
         * \param properties Property list
         * \param characters Character list
         *
         * Updates the display after the modification
         */
        void setLists(PropertyList *properties, CharacterList *characters);
        /*!
         * \brief Convert visual to logical row index
         * \param visualRow Visual row
         * \return Logical row
         */
        int logicalRow(int visualRow);
        /*!
         * \brief Convert visual to logical column index
         * \param visualColumn Visual column
         * \return Logical column
         */
        int logicalColumn(int visualColumn);
    signals:
        /*!
         * \brief Signal to register a modification
         * \param modification Modification to register
         *
         * This signal is sent when the tree is modified
         */
        void modificationDone(Modification *modification);
        /*!
         * \brief Signal to open a note
         * \param note Note we want to open
         *
         * This signal is sent when a note has to be opened
         */
        void noteToOpen(Note *note);
        /*!
         * \brief Signal to delete a note
         * \param note Note we want to delete
         *
         * This signal is sent when a note has to be deleted
         */
        void noteToDelete(Note *note);
    public slots:
        /*!
         * \brief Update of the display
         * \param row Row to focus on
         * \param column Column to focus on
         */
        void updateDisplay(int row = -1, int column = -1);
        /*!
         * \brief Scrolling
         * \param row Row index
         * \param column Column index
         *
         * Scroll to the item at the given indices. If one of the indices is equal to -1, no scrolling in this direction will be performed.
         */
        void scrollTo(int row, int column);
        /*!
         * \brief Update after a modification
         * \param modification Modification to undo or redo
         * \param undo True to undo, false to redo
         */
        void updateModification(CharacterModification *modification, bool undo);
};

#endif
