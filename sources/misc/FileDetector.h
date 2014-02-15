/*************************************************************************
* Copyright © 2013-2014 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_FILEDETECTOR
#define HEADER_FILEDETECTOR

#include <magic.h>
#include <string>

//! File type detector
class FileDetector
{
    public:
        //! Default constructor
        FileDetector(const std::string &installDir);
        //! Default destructor
        ~FileDetector();
        /*!
         * \brief Type detection
         * \param fileName Name of the file to be analysed
         * \return Type of the file
         */
        std::string typeOfFile(const std::string &fileName) const;
        /*!
         * \brief Getter for the validity of the detector
         * \return True if the detector is valid, false otherwise
         */
        bool isValid() const;
    private:
        //! Magic cookie
        magic_t mCookie;
        //! Buffer used when magic_file fails
        char *pBuffer;
};

#endif
