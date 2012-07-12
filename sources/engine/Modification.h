/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_MODIFICATION
#define HEADER_MODIFICATION

/*!
 * \brief Modification of a scenario
 */
class Modification
{
    public:
        /*!
         * \brief Type of modification
         */
        enum Type
        {
            //! Addition
            tAddition,
            //! Edition
            tEdition,
            //! Deletion
            tDeletion
        };
        /*!
         * \brief Location of the modification
         */
        enum Location
        {
            //! Metadata
            lMetadata,
            //! Scenario
            lScenario,
            //! History
            lHistory,
            //! Notes
            lNotes,
            //! Music
            lMusic,
            //! Characters
            lCharacters,
            //! Sound effects
            lFX
        };
        /*!
         *  \brief Constructor
         *  \param type Type of modification
         *  \param location Location of the modification
         */
        Modification(Type type, Location location);
        /*!
         * \brief Getter of the type
         * \return Type of the modification
         */
        Type type() const;
        /*!
         * \brief Getter of the location
         * \return Location of the modification
         */
        Location location() const;
    private:
        Type tType;
        Location lLocation;
};

#endif
