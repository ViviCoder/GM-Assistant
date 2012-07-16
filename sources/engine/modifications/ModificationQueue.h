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

#ifndef HEADER_MODIFICATIONQUEUE
#define HEADER_MODIFICATIONQUEUE

#include <vector>
#include "Modification.h"

/*!
 * \brief Queue of modifications
 */
class ModificationQueue
{
    public:
        /*!
         * \brief Constructor
         */
        ModificationQueue();
        /*!
         * \brief Destructor
         */
        ~ModificationQueue();
        /*!
         * \brief Clearing method
         *
         * Deletes all the modifications and clears the container
         */
        void clear();
        /*!
         * \brief Addition method
         * \param modification New modification
         *
         * Deletes undone modifications and adds the new
         */
        void add(Modification &newModification);
        /*!
         * \brief Undoing
         *
         * Undoes the last (done or redone) modification
         */
        void undo();
        /*!
         * \brief Redoing
         *
         * Redoes the last undone modification
         */
        void redo();
    private:
        //! Container for the modifications
        std::vector<Modification*> vModifs;
        //! Iterator pointing to the last modification
        std::vector<Modification*>::reverse_iterator iCurrent;
};

#endif
