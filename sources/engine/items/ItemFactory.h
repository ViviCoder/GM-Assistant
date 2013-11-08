/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_ITEMFACTORY
#define HEADER_ITEMFACTORY

#include "Item.h"
#include "FileItem.h"
#include "SoundItem.h"
#include "ImageItem.h"

/*!
 * \brief Item factory
 *
 * This class has only static methods designed to operate on items of different types
 */
class ItemFactory
{
    public:
        /*!
         * \brief Copy of an item
         * \param item Item to copy
         * \return Copy of the item
         *
         * Copies an item with respect to its type
         */
        static Item* copyItem(Item *item);
        /*!
         * \brief Creation of a new item
         * \param type Type of the item to create
         * \param content Content of the item to create
         * \param state State of the item to create
         * \param expanded Expanded/collapsed state of the item
         * \return New item
         *
         * Creates a new Item with the given type and the given parameters
         */
        static Item* createItem(Item::Type type, const std::string &content, Item::State state, bool expanded = false);
};

#endif
