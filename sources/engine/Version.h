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

#ifndef HEADER_VERSION
#define HEADER_VERSION

#define MAJOR_VERSION   1
#define MINOR_VERSION   2
#define RELEASE_VERSION 0

#include <string>

/*!
 * \brief Class containing version information
 */
class Version
{
    private:
        //! \brief Major version number
        int iMajor;
        //! \brief Minor version number
        int iMinor;
        //! \brief Release version number
        int iRelease;
    public:
        /*!
         * \brief Constructor
         * \param major Major version number
         * \param minor Minor version number
         * \param release Release version number
         */
        Version(int major = 1, int minor = 0, int release = 0);
        /*!
         * \brief Current version
         * \return Current version
         */
        static Version currentVersion();
        /*!
         * \brief Short version
         * \return String containing the short version
         *
         * The short version is in the form of MAJOR.MINOR
         */
        std::string shortVersion();
        /*!
         * \brief Full version
         * \return String containing the full version
         *
         * The full version is in the form of MAJOR.MINOR.RELEASE
         */
        std::string fullVersion();
};

#endif
