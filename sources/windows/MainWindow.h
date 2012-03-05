/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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
#include "Engine.h"
#include "SoundEngine.h"
#include "AboutDialog.h"
#include <QTimer>
#include <QSignalMapper>

// frequency of refreshing music information
#define TICK 10
// number of recently opened games stored
#define RECENT_NUMBER   5

/*!
 * \brief Main window of the application
 */
class MainWindow: public QMainWindow, private Ui::mainWindow
{
    private:
        /*!
         * \brief Game engine
         */
        Engine eGame;
        SoundEngine soundEngine;
        /*!
         * \brief File name of the current game
         */
        QString sFileName;
        bool bModified;
        AboutDialog *pAboutDial;
        /*!
         * \brief Timer used by the window
         */
        QTimer *timer;
        /*!
         * \brief Number of timer ticks
         *
         */
        int iTimerCount;
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
        QSignalMapper *smMapper;
        /*!
         * \brief Duration of the current music
         */
        double dDuration;
        /*!
         * \brief File name of the current music
         */
        std::string sCurrentMusic;
    public:
        /*!
         * \brief Constructor of the main window
         *
         * Initializes various things
         */
        MainWindow();
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
         * Change the layout to Simple (Music + Scenario Tree)
         */
        void on_actionSimple_triggered();

        /*!
         * \brief Slot for "Full" menu item
         * 
         * Change the layout to Full layout
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
         *
         * Saves the game in a new file
         */
        void on_actionS_ave_as_triggered();
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
        // button
        void on_buttonMusic_clicked();
        /*!
         * \brief Slot for when the timer ticks
         */
        void onTimer_timeout();
        /*!
         * \brief Display update
         *
         * Updates the display when the current game is changed or reloaded
         */
        void updateDisplay();
        // slider
        /*!
         * \brief Slot for when the slider is released (or clicked)
         */
        void on_sliderMusic_released();
        /*!
         * \brief Slot for when the slider is wheeled
         * \param positive True if wheeled forward, false otherwise
         */
        void on_sliderMusic_wheeled(bool positive);
        /*!
         * \brief Slot for when the value of the slider has changed
         * \param value New value of the slider
         */
        void on_sliderMusic_valueChanged(int value);
        // play music and sound
        /*!
         * \brief Play a music file
         * \param fileName File to play
         * \param duration Estimated duration of the music
         *
         * Plays the music file given only if its duration is greater then zero
         */
        void playMusic(const std::string &fileName, double duration);
        void playSound(const std::string &fileName);
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
         * Load the corresponding game
         */
        void loadRecent(int index);
    protected:
        // overriden methods
        /*!
         * \brief ResizeEvent handler
         * \param e Event raised
         *
         * Stores the old size for unmaximizing just after startup
         */
        void resizeEvent(QResizeEvent *e);
};

#endif
