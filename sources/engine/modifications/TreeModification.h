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
         * \brief Constructor for additions
         * \param tree Modified tree
         * \param indices Indices of the new item
         */
        TreeModification(Tree &tree, const std::string &indices);
        /*!
         * \brief Constructor for deletions
         * \param tree Modified tree
         * \param branch Deleted branch
         * \param indices Indices of the branch
         */
        TreeModification(Tree &tree, Branch *branch, const std::string &indices);
        /*!
         * \brief Constructor for full editions
         * \param tree Modified tree
         * \param oldItem Previous item
         * \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item *oldItem, const std::string &indices);
        /*!
         * \brief Constructor for content-only editions
         * \param tree Modified tree
         * \param content Previous content
         * \param newContent New content
         * \param indices Indices of the item
         */
        TreeModification(Tree &tree, const std::string &content, const std::string &newContent, const std::string &indices);
        /*!
         * \brief Constructor for state-only editions
         * \param tree Modified tree
         * \param state Previous state
         * \param newState New state
         * \param indices Indices of the item
         */
        TreeModification(Tree &tree, Item::State state, Item::State newState, const std::string &indices);
        /*!
         * \brief Constructor for movements
         * \param tree Modified tree
         * \param indices Indices of the moved branch
         * \param newIndices New indices of the branch
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
         * \brief Getter for the old version of a newly edited item
         * \return Old item
         */
        inline Item* item() const;
        /*!
         * \brief Getter for the deleted branch
         * \return Deleted branch
         */
        inline Branch* branch() const;
        /*!
         * \brief Getter for the item to be undone (for additions and full editions)
         * \return Item to be undone (to be deleted separately by freeItem)
         */
        inline Item* undoneItem() const;
        /*!
         * \brief Getter for the edition type (if edition)
         * \return Edition type (if edition only)
         */
        inline EditionType editionType() const;
        /*!
         * \brief Getter for the current copy of the item (for editions)
         * \return Current item
         *
         * Is non null only after undo has been called at least once
         */
        inline Item* currentItem() const;
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
        //! Copy of the modified item
        Item *pItem;
        //! Previous content
        std::string sContent;
        //! New content
        std::string sNewContent;
        //! Previous state
        Item::State sState;
        //! New state
        Item::State sNewState;
        //! Item to be undone (for undoing additions and full editions)
        Item *pUndoneItem;
        //! Current copy of the item (for editions)
        Item *pCurrentItem; 
        //! Flag to indicate if a full edition conserves the item type
        bool bSameType;
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

Branch* TreeModification::branch() const
{
    return pBranch;
}

Item* TreeModification::undoneItem() const
{
    return pUndoneItem;
}

TreeModification::EditionType TreeModification::editionType() const
{
    return etEditType;
}

Item* TreeModification::currentItem() const
{
    return pCurrentItem;
}

#endif
