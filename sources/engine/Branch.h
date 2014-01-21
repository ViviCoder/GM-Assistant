/*************************************************************************
* Copyright © 2011-2014 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_BRANCH
#define HEADER_BRANCH

#include "Tree.h"

class Branch
{
    private:
        Item *pItem;
        Tree tTree;
        Tree *pParent;
    public:
        // constructors
        Branch(const Branch &branch, Tree *parent=0);
        Branch(Item* item, Tree *parent=0);
        Branch(Item* item, const Tree &tree, Tree *parent=0);
        /*!
         * \brief Constructor from an XML tree
         * \param item Root item of the branch
         * \param config IO configuration
         * \param root Root of the XML tree
         * \param checkFiles Indicates if the existence of the files within FileItems is checked
         * \param parent Parent tree
         */
        Branch(Item* item, const IOConfig &config, const xmlpp::Element &root, bool checkFiles, Tree *parent);
        // destructor
        ~Branch();
        /*!
         * \brief Getter for the item
         * \return Item of the branch
         */
        inline Item* item() const;
        /*!
         * \brief Getter for the child tree
         * \return Child tree of the branch
         */
        inline Tree& tree();
        /*!
         * \brief Getter for the parent tree
         * \return Parent tree of the branch
         */
        inline Tree* parent() const;
        /*!
         * \brief Setter for the parent tree
         * \param parent New parent tree
         */
        inline void setParent(Tree *parent);
        // copy operator
        Branch& operator=(const Branch &branch);
        /*!
         * \brief Setter for the item
         * \param item New item
         */
        inline void setItem(Item *item);
};

Item* Branch::item() const
{
    return pItem;
}

Tree& Branch::tree()
{
    return tTree;
}

Tree* Branch::parent() const
{
    return pParent;
}

void Branch::setParent(Tree *parent)
{
    pParent = parent;
}

void Branch::setItem(Item *item)
{
    pItem = item;
}

#endif
