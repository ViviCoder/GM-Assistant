/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_QSOUNDENGINE
#define HEADER_QSOUNDENGINE

#include <phonon>

// frequency of refreshing music information
#define TICK_INTERVAL 100

/*!
 * \brief Sound engine
 */
class QSoundEngine: public QObject
{
    Q_OBJECT
	private:
        //! Music audio object
        Phonon::MediaObject *musicObject;
        //! Sound audio object
        Phonon::MediaObject *soundObject;
	public:
		/*!
         * \brief Default constructor
         * \param parent Parent widget
         */
        QSoundEngine(QWidget *parent = 0);
		// accessors
        bool isPlayingMusic() const;
        bool isMusicPaused() const;
        /*!
         * \brief Getter for the music object
         * \return The the music object
         */
        Phonon::MediaObject* musicPlayer() const;
		/*!
         * \brief Play the given sound
         * \param fileName Name of the file to play
         */
        void playSound(const QString &fileName);
		/*!
         * \brief Play the given song
         * \param fileName Name of the file to play
         */
		void playMusic(const QString &fileName);
        void pauseMusic();
        void resumeMusic();
        void stop();
        void stopMusic();
        void move(double step);
        /*!
         * \brief Getter for the duration of the playing song
         * \return Duration of the current song (-1 if none)
         */
        int musicDuration() const;
    private slots:
        /*!
         * \brief Slot for when the music state changes
         * \param newState New state
         * \param oldState Old state
         */
        void onMusicStateChanged(Phonon::State newState, Phonon::State oldState);
        /*!
         * \brief Slot for when the music state changes
         * \param newState New state
         * \param oldState Old state
         */
        void onSoundStateChanged(Phonon::State newState, Phonon::State oldState);
    signals:
        /*!
         * \brief Signal send when a fatal error occurs
         * \param message Error message
         */
        void errorOccured(const QString &message);
};

#endif
