/*************************************************************************
* Copyright © 2012-2014 Vincent Prat & Simon Nicolas
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
         * \brief Type of edition
         */
        enum EditionType
        {
            //! Full edition
            etFull,
            //! Content-only edition
            etContent,
            //! State-only edition
            etState
        };
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
         *  \brief Constructor for full editions
         *  \param tree Modified tree
         *  \param item Copy of the previous item
         *  \param newItem Copy of the new item
         *  \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item *item, Item *newItem, const std::string &indices);
        /*!
         *  \brief Constructor for content-only editions
         *  \param tree Modified tree
         *  \param content Previous content
         *  \param newContent New content
         *  \param indices Indices of the item
         */
        TreeModification(Tree &tree, const std::string &content, const std::string &newContent, const std::string &indices);
        /*!
         *  \brief Constructor for state-only editions
         *  \param tree Modified tree
         *  \param state Previous state
         *  \param newState New state
         *  \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item::State state, Item::State newState, const std::string &indices);
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
        // inherited pure virtual getter
        inline Type type() const;
        /*!
         * \brief Getter for the tree
         * \return Modified tree
         */
        inline Tree& tree();
        // inherited pure virtual methods
        void undo();
        void redo();
        /*!
         * \brief Getter for the indices
         * \return Indices of the modification
         */
        inline std::string indices() const;
        /*!
         * \brief Modified indices for undoing a movement
         * \return Indices where to put back the moved branch
         */
        std::string modifiedIndices() const;
        /*!
         * \brief Modified new indices for undoing a movement
         * \return Indices of the moved branch after the movement
         */
        std::string modifiedNewIndices() const;
        /*!
         * \brief Modified indices for deletions/additions
         * \return Indices of the nearest item to the deleted one
         */
        std::string deletedIndices() const;
        /*!
         * \brief Getter for the modified item (or the previous when undoing an addition)
         * \return Modified item or just undone added item (to be deleted separately by freeItem))
         */
        inline Item* item() const;
        /*!
         * \brief Getter for the newly added item
         * \return New item
         */
        inline Item* newItem() const;
        /*!
         * \brief Unadded item deletion
         */
        void freeItem();
    private:
        //! Type of edition (if this is the case)
        EditionType etEditType;
        //! Indices of the modification
        std::string sIndices;
        //! Indices of the destination
        std::string sNewIndices;
        //! Tree modified
        Tree &rTree;
        //! Copy of the deleted branch
        Branch *pBranch;
        //! Copy of the modified item (or the previous item when undoing an addition)
        Item *pItem;
        //! Copy of the new item
        Item *pNewItem;
        //! Previous content
        std::string sContent;
        //! New content
        std::string sNewContent;
        //! Previous state
        Item::State sState;
        //! New state
        Item::State sNewState;
};

Modification::Type TreeModification::type() const
{
    return tTree;
}

std::string TreeModification::indices() const
{
    return sIndices;
}

Tree& TreeModification::tree()
{
    return rTree;
}

Item* TreeModification::item() const
{
    return pItem;
}

Item* TreeModification::newItem() const
{
    return pNewItem;
}

#endif
