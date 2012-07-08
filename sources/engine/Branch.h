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
        Branch(const Branch &branch, Tree *parent=NULL);
        Branch(Item* item, Tree *parent=NULL);
        Branch(Item* item, const Tree &tree, Tree *parent=NULL);
        /*!
         * \brief Constructor from an XML tree
         * \param item Root item of the branch
         * \param root Root of the XML tree
         * \param checkFiles Indicates if the existence of the files within FileItems is checked
         * \param parent Parent tree
         */
        Branch(Item* item, const xmlpp::Element &root, bool checkFiles, Tree *parent);
        // destructor
        ~Branch();
        //accessors
        Item* item() const;
        Tree& tree();
        Tree* parent() const;
        void setParent(Tree *parent);
        // copy operator
        Branch& operator=(const Branch &branch);
        /*!
         * \brief Setter for the item
         * \param item New item
         *
         * Replaces the old item by the new one and deletes it
         */
        void setItem(Item *item);
};

#endif
