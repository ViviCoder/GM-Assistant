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
         *  \brief Constructor for additions
         *  \param tree Modified tree
         *  \param newItem Copy of the new item
         *  \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item *newItem, const std::string &indices);
        /*!
         *  \brief Constructor for deletions
         *  \param tree Modified tree
         *  \param branch Copy of the deleted branch
         *  \param indices Indices of the branch
         */
        TreeModification(Tree &tree, Branch *branch, const std::string &indices);
        /*!
         *  \brief Constructor for editions
         *  \param tree Modified tree
         *  \param item Copy of the previous item
         *  \param newItem Copy of the new item
         *  \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item *item, Item *newItem, const std::string &indices);
        /*!
         *  \brief Constructor for movements
         *  \param tree Modified tree
         *  \param indices Indices of the moved branch
         *  \param newIndices New indices of the branch
         */
        TreeModification(Tree &tree, const std::string &indices, const std::string &newIndices);
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
        //! Copy of the deleted branch
        Branch *pBranch;
        //! Copy of the modified item
        Item *pItem;
        //! Copy of the new item
        Item *pNewItem;
        /*!
         * \brief Modified indices for undoing a movement
         * \return New indices of the moved branch after the movement
         */
        std::string modifiedIndices() const;
};

#endif
