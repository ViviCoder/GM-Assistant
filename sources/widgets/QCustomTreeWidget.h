/*************************************************************************
* Copyright © 2011-2014 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>
#include "Tree.h"
#include "ItemDialog.h"
#include "QSoundEngine.h"
#include "TreeModification.h"

class QCustomTreeWidgetItem;
/*!
 * \brief Custom tree widget
 *
 * This widget is designed to manipulate an underlying Tree
 */
class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    public:
        /*!
         * \brief Method to play audio files
         */
        enum PlayingMethod
        {
            //! Audio files will not be played 
            pmNone, 
            //! Audio files will be played like sounds
            pmSound,
            //! Audio files will be played like songs
            pmMusic
        };
        /*!
         * \brief Constructor of the widget
         * \param parent Parent widget
         *
         * Creates its pop-up menu and connects various slots
         */
        QCustomTreeWidget(QWidget *parent=0);
        ~QCustomTreeWidget();
        /*! 
         * \brief Setter for the tree of the widget
         * \param tree Pointer to the new tree
         *
         * Associates the tree to the widget and updates its display
         */
        void setTree(Tree *tree);
        // associating an icon to a state
        static QIcon icon(Item::State state);
        /*!
         * \brief Setter for the playing method
         * \param player Widget in charge of playing
         * \param playingMethod New playing method
         */
        void setPlayingMethod(QWidget *player, PlayingMethod playingMethod);
        /*!
         * \brief Getter for the playing method
         * \return Playing method
         */
        PlayingMethod playingMethod() const;
        /*!
         * \brief Setter for the item dialog window
         * \param window Item dialog window
         */
        void setItemDialogWindow(ItemDialog *window);
    public slots:
        void on_itemChanged(QTreeWidgetItem* item,int column);
        /*!
         * \brief Deletes an item
         * \param item Item to delete
         */
        void deleteItem(QTreeWidgetItem* item);
        void on_itemSelectionChanged();
        /*!
         * \brief Slot for "Add" menu action
         * \param item Current item from which the new one will be added (or the one which will be edited
         * \param edition Indicates if it is an edition (true) or not
         * 
         * Opens the item dialog window and adds the newly created item
         */
        void addItem(QCustomTreeWidgetItem* item, bool edition = false);
        /*!
         * \brief Slot for double click and space key pressed
         * \param qItem Current item which will be "launched"
         *
         * Executes the action associated with the type of the item
         */
        void launchItem(QTreeWidgetItem* qItem);
        /*!
         * \brief Update of the display
         * \param indices Indices of the item to get focus
         */
        void updateDisplay(const std::string &indices = "");
        /*
         * \brief Scrolling
         * \param item Item to scroll to
         */
        void scrollTo(QTreeWidgetItem *item);
        /*!
         * \brief Update of the display after a modification
         * \param modification Modification to undo or redo
         * \param undo True to undo, false to redo
         */
        void updateModification(TreeModification *modification, bool undo);
        /*!
         * \brief Modifies the state of an item
         * \param item Item to modify
         * \param state New state
         */
        void changeState(QCustomTreeWidgetItem *item, Item::State state);
        /*!
         * \brief Slot for exporting files
         * \param item Item which contains the file to export
         */
        void exportFile(Item *item);
    protected:
        // overriden methods
        /*!
         * \brief DoubleClickEvent handler
         * \param e Mouse event
         */
        void mouseDoubleClickEvent(QMouseEvent *e);
        /*!
         * \brief Event raised when a mouse button is pressed
         * \param e Mouse event
         */
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        /*!
         * \brief Event raised when a key is released
         * \param e Key event
         */
        void keyReleaseEvent(QKeyEvent *e);
        void dragEnterEvent(QDragEnterEvent *e);
        /*!
         * \brief DropEvent handler
         * \param e Event to handle
         */
        void dropEvent(QDropEvent *e);
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         */
        void changeEvent(QEvent *e);
        /*!
         * \brief Retranslator
         *
         * Retranslates the menu
         */
        void retranslate();
    protected slots:
        /*!
         * \brief Slot for when an item is expanded
         * \param item Newly expanded item
         *
         * Modifies the underlying item to remember it is expanded
         */
        void onItemExpanded(QTreeWidgetItem *item);
        /*!
         * \brief Slot for when an item is collapsed
         * \param item Newly collapsed item
         *
         * Modifies the underlying item to remember it is collapsed
         */
        void onItemCollapsed(QTreeWidgetItem *item);
    private:
        /*!
         * \brief Pop-up menu
         */
        QMenu *menuIcons;
        /*!
         * \brief Untag action
         */
        QAction *actionNone;
        /*!
         * \brief "In progress" tag action
         */
        QAction *actionProgress;
        /*!
         * \brief "Failed" tag action
         */
        QAction *actionFailure;
        /*!
         * \brief "Succeeded" tag action
         */
        QAction *actionSuccess;
        /*!
         * \brief Add action
         */
        QAction *actionAdd;
        /*!
         * \brief Delete action
         */
        QAction *actionDelete;
        /*!
         * \brief Edit action
         */
        QAction *actionEdit;
        /*!
         * \brief Launch action
         */
        QAction *actionLaunch;
        //! Export action
        QAction *actionExport;
        /*!
         * \brief Underlying tree
         */
        Tree *pTree;
        /*!
         * \brief Dialog window used to create items
         */
        ItemDialog *pItemDial;
        QTreeWidgetItem *pDragSource;
        bool bNewlySelected;
        bool bEditing;
        /*!
         * \brief Playing method
         */
        PlayingMethod pmMethod;
        /*!
         * \brief Method to stop music if the playing sound file is in a given branch
         * \param branch Branch where the item to stop would be
         */
        void stopMusic(Branch *branch);
    signals:
        /*!
         * \brief Signal to play a file
         * \param item Pointer to the soundItem we want to play
         *
         * This signal is sent when a sound file has to be played
         */
        void fileToPlay(const SoundItem *item);
        /*!
         * \brief Signal to register a modification
         * \param modification Modification to register
         *
         * This signal is sent when the tree is modified
         */
        void modificationDone(Modification *modification);
        /*!
         * \brief Signal to stop playing a file
         * \param item Pointer to the soundItem we want to stop
         */
        void fileToStop(const SoundItem *item);
        /*!
         * \brief Signal to change the current sound item (without playing it again)
         * \param oldItem Old sound item
         * \param newItem New sound item
         */
        void fileToChange(const SoundItem *oldItem, const SoundItem *newItem);
};

#endif
