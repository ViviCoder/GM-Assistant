/*************************************************************************
* Copyright © 2012-2018 Vincent Prat & Simon Nicolas
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
         * Deletes undone modifications and adds the new one
         */
        void add(Modification *modification);
        /*!
         * \brief Undoing
         * \return Just undone modification (0 if none)
         *
         * Undoes the last (done or redone) modification
         */
        Modification* undo();
        /*!
         * \brief Redoing
         * \return Just redone modification (0 if none)
         *
         * Redoes the last undone modification
         */
        Modification* redo();
        /*!
         * \brief Test of undo-ability
         * \return True if there is something to undo, false otherwise
         */
        bool undoable() const;
        /*!
         * \brief Test of redo-ability
         * \return True if there is something to redo, false otherwise
         */
        bool redoable() const;
        /*!
         * \brief Test if the last modification has been saved
         * \return True if the last modification has been saved, false otherwise
         */
        bool isUpToDate() const;
        /*!
         * \brief Save the last modification
         */
        void save();
    private:
        //! Container for the modifications
        std::vector<Modification*> vModifs;
        //! Number of applied modification
        int iCurrent;
        //! Number of saved modification
        int iSaved;
};

inline bool ModificationQueue::undoable() const
{
    return (iCurrent > 0);
}

inline bool ModificationQueue::redoable() const
{
    return (iCurrent < vModifs.size());
}

inline bool ModificationQueue::isUpToDate() const
{
    return (iCurrent == iSaved);
}

inline void ModificationQueue::save()
{
    iSaved = iCurrent;
}

#endif
