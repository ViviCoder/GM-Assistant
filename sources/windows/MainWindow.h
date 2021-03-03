/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_MAINWINDOW
#define HEADER_MAINWINDOW

#include "ui_MainWindow.h"
#include "Scenario.h"
#include "AboutDialog.h"
#include "DiceDialog.h"
#include "SelectCharacterDialog.h"
#include <QTimer>
#include <QSignalMapper>
#include "ModificationQueue.h"
#include "ItemDialog.h"
#include "MetadataDialog.h"
#include <QtMultimedia/QMediaPlayer>
#include "ReleaseNotesDialog.h"

// number of recently opened games stored
#define RECENT_NUMBER   5

/*!
 * \brief Main window of the application
 */
class MainWindow: public QMainWindow, private Ui::mainWindow
{
    private:
        //! Music player
        QMediaPlayer *musicPlayer;
        //! Sound player
        QMediaPlayer *soundPlayer;
        /*!
         * \brief File name of the current game
         */
        QString sFileName;
        //! Information dialog window
        AboutDialog *pAboutDial;
        //! Dice simulation dialog window
        DiceDialog *pDiceDialog;
        //! Character selection dialog window
        SelectCharacterDialog *pSelectCharacterDialog;
        //! Combat manager dialog window
        CombatDialog *pCombatDialog;
        /*!
         * \brief Previous size of the window before being maximized
         */
        QSize sUnmaximizedSize;
        /*!
         * \brief List of recently opened games
         */
        QStringList slRecent;
        /*!
         * \brief Signal mapper for actions related to recently opened games
         */
        QSignalMapper *smRecent;
        /*!
         * \brief SoundItem of the current music
         */
        const SoundItem *siCurrentMusic;
        //! Queue of modifications
        ModificationQueue mqQueue;
        //! Application translator
        QTranslator *tApplication;
        //! System translator
        QTranslator *tSystem;
        //! Installation directory
        QString sInstall;
        //! Signal mapper for the available languages
        QSignalMapper *smLanguage;
        //! Metadata editor
        MetadataDialog *pMetadataDialog;
        //! Scenario
        Scenario sGame;
        //! Item editor
        ItemDialog *pItemDialog;
        //! Release notes displayer
        ReleaseNotesDialog *pReleaseNotesDialog;
        /*!
         * \brief Game save
         * \param askForUpdate Indicates if the user should be asked to update an outdated game
         *
         * Saves the game
         */
        void save(bool askForUpdate = true);
        /*!
         * \brief Modification test
         * \return True if there are modifications to save
         *
         * Tests if the game has been modified since the last save
         */
        inline bool isModified() const;
    protected:
        // overriden methods
        /*!
         * \brief ResizeEvent handler
         * \param e Event raised
         *
         * Stores the old size for unmaximizing just after startup
         */
        void resizeEvent(QResizeEvent *e) override;
        /*!
         * \brief Event filter
         * \param source Source of the event
         * \param e Event
         *
         * Intercepts locally handled events related to global shortcuts
         */
        bool eventFilter(QObject *source, QEvent *e) override;
        /*!
         * \brief CloseEvent handler
         * \param e Event to handle
         *
         * Checks unsaved modifications before closing
         */
        void closeEvent(QCloseEvent *e) override;
        /*!
         * \brief ChangeEvent handler
         * \param e Event to handle
         *
         * Updates the translation when changing language
         */
        void changeEvent(QEvent *e) override;
    public:
        /*!
         * \brief Constructor of the main window
         * \param install_dir Installation directory
         *
         * Initializes various things
         */
        MainWindow(const QString &install_dir);
        /*!
         * \brief Destructor of the main window
         *
         * Saves window's display parameters and other options before closing the application
         */
        ~MainWindow();

    Q_OBJECT
    private slots:
        // actions
        /*!
         * \brief Slot for cleaning the layout
         */
        void clearLayout();
        /*!
         * \brief Slot for "Music" menu item
         * 
         * Change the layout to Music
         */
        void on_actionMusic_triggered();

        /*!
         * \brief Slot for "Simple" menu item
         * 
         * Change the layout to Simple
         */
        void on_actionSimple_triggered();

        /*!
         * \brief Slot for "Full" menu item
         * 
         * Change the layout to Full
         */
        void on_actionFull_triggered();

        /*!
         * \brief Slot for "Design" menu item
         * 
         * Change the layout to Design
         */
        void on_actionDesign_triggered();

        /*!
         * \brief Slot for "NoMusic" menu item
         * 
         * Change the layout to No Music 
         */
        void on_actionNoMusic_triggered();

        void on_actionAbout_triggered();
        /*!
         * \brief Slot for the "Quit" menu item
         *
         * Closes the application
         */
        void on_action_Quit_triggered();
        /*!
         * \brief Slot for the "Load" menu item
         *
         * Loads a game selected in a dialog window
         */
        void on_action_Load_triggered();
        /*!
         * \brief Slot for the "Save" menu item
         *
         * Saves the game
         */
        void on_action_Save_triggered();
        /*!
         * \brief Slot for the "Save as" menu item
         * \return True if a file has been chosen and saved
         *
         * Saves the game in a new file
         */
        bool on_actionS_ave_as_triggered();
        /*!
         * \brief Slot for the "New" menu item
         *
         * Opens an empty game
         */
        void on_action_New_triggered();
        /*!
         * \brief Slot for the "Reload" menu item
         *
         * Reloads the current game
         */
        void on_action_Reload_triggered();
        /*!
         * \brief Slot for the "Dice simulator" menu item
         *
         * Launches the dice simulator
         */
        void on_action_Dice_triggered();
        /*!
         * \brief Slot for the "Combat manager" menu item
         *
         * Launches the character selection dialog window before the combat manager
         */
        void on_action_Combat_triggered();
        /*!
         * \brief Slot for when the music button is clicked
         *
         * Play, pause or resume the current music
         */
        void on_buttonMusic_clicked();
        /*!
         * \brief Time display update in the music player
         */
        void updateTimeDisplay();
        /*!
         * \brief Display update
         *
         * Updates the display when the current game is changed or reloaded
         */
        void updateDisplay();
        // play music and sound
        /*!
         * \brief Play a music file associated with a SoundItem
         * \param item Pointer to the soundItem
         *
         * Plays the music file only if its duration is greater then zero
         */
        void playMusic(const SoundItem *item);
        /*!
         * \brief Stop the music of a soundItem
         * \param item Pointer to the soundItem we want to stop
         */
        void stopMusic(const SoundItem *item);
        /*!
         * \brief Play a sound file associated with a SoundItem
         * \param item Pointer to the soundItem
         */
        void playSound(const SoundItem *item);
        /*!
         * \brief Recent files update when opening a new file
         * \param fileName File to be opened
         *
         * Adds the file to the list of the recently opened games
         */
        void addRecent(const QString &fileName);
        /*!
         * \brief Recent files list update
         *
         * Deletes non-existent files in the list of recently opened files and limits their number
         */
        void updateRecent();
        /*!
         * \brief Slot for the "Recent" submenu items
         * \param index Index of the action triggered
         *
         * Loads the corresponding game
         */
        void loadRecent(int index);
        /*!
         * \brief Slot for the Undo menu item
         *
         * Undoes the last modification
         */
        void on_action_Undo_triggered();
        /*!
         * \brief Slot for the Redo menu item
         *
         * Redoes the last undone modification
         */
        void on_action_Redo_triggered();
        /*!
         * \brief Slot for new modifications
         * \param modification Modification to register
         *
         * Pushes the new modification into the queue
         */
        void registerModification(Modification *modification);
        /*!
         * \brief Slot for the update of undo/redo actions
         *
         * Enables or disables undo/redo actions
         */
        void updateUndoRedo();
        /*!
         * \brief Slot for the update after a modification
         * \param modification Modification which has just been made
         * \param undo True if undoing, false if redoing
         *
         * Updates the display of the right widget
         */
        void updateModification(Modification *modification, bool undo);
        /*!
         * \brief Test before closing a game
         * \return True if the user wants to close
         *
         * If the game has been modified since the last save, the user is asked confirmation before closing
         */
        bool canClose();
        /*!
         * \brief Slot for when a language is selected
         * \param suffix Suffix of the language
         *
         * Retranslates the interface
         */
        void translationRequested(const QString &suffix);
        /*!
         * \brief Slot for error messages from the music player
         * \param error Error to display
         */
        void displayError(QMediaPlayer::Error error);
        /*!
         * \brief Slot for the Metadata menu item
         *
         * Displays the metadata editor
         */
        void on_action_Metadata_triggered();
        /*!
         * \brief Slot used to change a file format if needed
         * \param askForUpdate Indicates if the user is asked to update an outdated game
         *
         * If a new file format is needed to save the game, it asks for a new file.
         * If askForUpdate is false, it updates the game.
         */
        void changeFormatIfNeeded(bool askForUpdate = true);
        /*!
         * \brief Slot to change the current sound item without modifying the playback
         * \param oldItem Old sound item
         * \param newItem New sound item
         */
        void changeCurrentMusic(const SoundItem *oldItem, const SoundItem *newItem);
        /*!
         * \brief Slot used whenever the state of the music player changes
         * \param state New state of the music player
         *
         * It is used to detect when the playback starts and stops
         */
        void onMusicStateChanged(QMediaPlayer::State state);
        /*!
         * \brief Slot used when the position in the music player has changed
         * \param position
         */
        void onMusicPositionChanged(qint64 position);
        /*!
         * \brief Slot used when the duration in the music player has changed
         * \param duration
         */
        void onMusicDurationChanged(qint64 duration);
        /*!
         * \brief Slot used when the slider is moved
         * \param position New position
         */
        void on_sliderMusic_sliderMoved(int position);
        /*!
         * \brief Slot used when a note is opened
         * \param note Editor for the note
         */
        void connectNote(QCustomTextEdit *note);
        /*!
         * \brief Slot for the Release notes menu item
         *
         * Displays the release notes
         */
        void on_action_Release_notes_triggered();
};

bool MainWindow::isModified() const
{
    return !mqQueue.isUpToDate() || tabNotes->unregisteredModification();
}

#endif
