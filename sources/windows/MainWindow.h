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
        Engine eGame;
        SoundEngine soundEngine;
        /*!
         * \brief File name of the current game
         */
        QString sFileName;
        QString sDir;
        bool bModified;
        AboutDialog *pAboutDial;
        QTimer *timer;
        int iTimerCount;
        /*!
         * \brief Previous size of the window before being maximized
         */
        QSize sUnmaximizedSize;
        /*!
         * \brief List of recently opened games
         */
        QStringList slRecent;
    public:
        /*!
         * \brief Constructor of the main window
         * \param dir   working directory
         *
         * Initializes various things
         */
        MainWindow(const QString &dir);
        /*!
         * \brief Destructor of the main window
         *
         * Saves window's display parameters and other options before closing the application
         */
        ~MainWindow();

    Q_OBJECT
    private slots:
        // actions
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
        // timer
        void onTimer_timeout();
        /*!
         * \brief Display update
         *
         * Updates the display when the current game is changed or reloaded
         */
        void updateDisplay();
        // slider
        void on_sliderMusic_sliderReleased();
        // play music and sound
        void playMusic(const std::string &fileName);
        void playSound(const std::string &fileName);
        /*!
         * \brief Recent files update
         * \param fileName File to be opened
         *
         * Updates the list of the recently opened games
         */
        void updateRecent(const QString &fileName);
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
