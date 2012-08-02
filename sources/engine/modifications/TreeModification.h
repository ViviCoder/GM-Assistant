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

#ifndef HEADER_TREEMODIFICATION
#define HEADER_TREEMODIFICATION

#include "Modification.h"
#include "Tree.h"

/*!
 * \brief Modification of a tree
 */
class TreeModification: public Modification
{
    public:
        /*!
         *  \brief Constructor
         *  \param action Action
         *  \param tree Modified tree
         *  \param branch Modified branch
         *  \param indices Indices of the modification
         *  \param newIndices New indices for moves
         */
        TreeModification(Action action, Tree &tree, Branch branch, const std::string &indices, const std::string &newIndices = "");
        /*!
         * \brief Destructor
         */
        virtual ~TreeModification();
        /*!
         * \brief Getter for the type
         * \return Type of the modification
         */
        Type type() const;
        /*!
         * \brief Getter for the tree
         * \return Modified tree
         */
        Tree& tree();
        // inherited pure virtual methods
        void undo();
        void redo();
        /*!
         * \brief Getter for the indices
         * \return Indices of the modification
         */
        std::string indices() const;
    private:
        //! Indices of the modification
        std::string sIndices;
        //! Indices of the destination
        std::string sNewIndices;
        //! Tree modified
        Tree &rTree;
        //! Content of the modification
        Branch bBranch;
};

#endif
