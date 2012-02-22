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

#ifndef HEADER_SOUNDITEM
#define HEADER_SOUNDITEM

#include "FileItem.h"
#include "QCustomThread.h"

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
         * \brief Boolean indicating whether the computation of the duration is finished
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
         */
        void setFileName(const std::string &fileName);
    public:
        /*!
         *  \brief Constructor
         *  \param content Content of the item
         *  \param state State of the item
         *  \param fileName Name of the sound file of the item
         */
        SoundItem(const std::string &content="", State state=sNone, const std::string &fileName="");
        /*!
         * \brief Destructor
         */
        ~SoundItem();
        /*!
         * \brief Read the item from a XML file
         * \param root Parent XML element
         * \throw xmlpp::exception Exception thrown when the XML file is uncorrect
         */
        void fromXML(const xmlpp::Element &root) throw(xmlpp::exception);
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
