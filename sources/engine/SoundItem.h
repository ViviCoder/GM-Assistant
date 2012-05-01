/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_SOUNDITEM
#define HEADER_SOUNDITEM

#include "FileItem.h"
#include "QCustomThread.h"

#define SOUND_SIZE_LIMIT    1024

/*!
 * \brief Item related to a sound file 
 */
class SoundItem: public FileItem
{
    private:
        /*!
         * \brief Duration of the sound
         */
        double dDuration;
        /*!
         * \brief Boolean indicating if the computation of the duration is finished
         */
        bool bThreadFinished;
        /*!
         * \brief Thread used to compute the duration
         */
        QCustomThread *pThread;
    protected:
        /*!
         * \brief Setter for the file name
         * \param fileName New file name
         * \param checkFile Indicates if the existence of the file is checked
         * \throw std::invalid_argument Thrown when the file is not readable
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         *
         * Calculates the duration of the sound file
         */
        void setFileName(const std::string &fileName, bool checkFile = true) throw(std::invalid_argument, std::overflow_error);
    public:
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param fileName Name of the sound file of the item
         *  \param sizeLimited Indicates wether or not the size of the file is limited
         */
        SoundItem(const std::string &content="", State state=sNone, const std::string &fileName="", bool sizeLimited = false);
        /*!
         * \brief Destructor
         */
        ~SoundItem();
        /*!
         * \brief Read the item from a XML file
         * \param root Parent XML element
         * \param checkFile Indicates if the existence of the file is checked
         * \throw xmlpp::exception Exception thrown when the XML file is uncorrect
         * \throw std::invalid_argument Exception thrown when the sound file does not exist
         * \throw std::overflow_error Thrown when the size of the file exceeds the limit (if limited)
         */
        void fromXML(const xmlpp::Element &root, bool checkFile = true) throw(xmlpp::exception, std::invalid_argument, std::overflow_error);
        /*!
         * \brief Getter for the type of the item
         * \return Item::tSound
         */
        Type type() const;  // overriden
        /*!
         * \brief Getter for the duration of the sound
         * \return Duration of the sound
         */
        double duration() const;
};

#endif
