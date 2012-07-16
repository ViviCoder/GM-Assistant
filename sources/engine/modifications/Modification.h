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
         * \brief Action
         */
        enum Action
        {
            //! Addition
            aAddition,
            //! Edition
            aEdition,
            //! Deletion
            aDeletion,
            //! Movement
            aMovement
        };
        /*!
         * \brief Type of modification
         */
        enum Type
        {
            //! Metadata
            tMetadata,
            //! Tree
            tTree,
            //! Character
            tCharacter,
            //! Note
            tNote
        };
        /*!
         *  \brief Default constructor
         *  \param action Action
         */
        Modification(Action action);
        /*!
         * \brief Default destructor
         */
        virtual ~Modification();
        /*!
         * \brief Getter of the action
         * \return Action
         */
        Action action() const;
        /*!
         * \brief Getter for the type
         * \return Type of the modification
         */
        virtual Type type() const = 0;
        /*!
         * \brief Undoing
         */
        virtual void undo() = 0;
        /*!
         * \brief Redoing
         */
        virtual void redo() = 0;
    private:
        //! \brief Action
        Action aAction;
};

#endif
