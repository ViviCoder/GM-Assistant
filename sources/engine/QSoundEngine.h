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

#include <string>
#include <stdexcept>
#include <phonon>

#define DEFAULT_BUFFER_SIZE 1024

/*!
 * \brief Sound engine
 */
class QSoundEngine
{
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
        // methods
		void playSound(const std::string &fileName) throw(std::runtime_error);
		void playMusic(const std::string &fileName) throw(std::runtime_error);
        void pauseMusic();
        void resumeMusic();
        void stop();
        void stopMusic();
        void move(double step);
};

#endif
