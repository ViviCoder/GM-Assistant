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

class MainWindow: public QMainWindow, private Ui::mainWindow
{
    private:
        Engine eGame;
        SoundEngine soundEngine;
        QString sFileName;
        bool bModified;
        AboutDialog *pAboutDial;
        QTimer *timer;
        int iTimerCount;
    public:
        // constructor
        MainWindow();

    Q_OBJECT
    private slots:
        // actions
        void on_actionAbout_triggered();
        void on_action_Quit_triggered();
        void on_action_Load_triggered();
        void on_action_Save_triggered();
        void on_actionS_ave_as_triggered();
        void on_action_New_triggered();
        // textNotes
        void on_textNotes_textChanged();
        // button
        void on_buttonMusic_clicked();
        // timer
        void onTimer_timeout();
        // general slots
        void updateDisplay();
        // slider
        void on_sliderMusic_sliderReleased();
    public slots:
        // play music and sound
        void playMusic(const std::string &fileName);
        void playSound(const std::string &fileName);
};

#endif
