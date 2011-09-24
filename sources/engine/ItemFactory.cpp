/*************************************************************************
* Copyright © 2011 Free Software Fundation
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

#include "ItemFactory.h"

Item* ItemFactory::copyItem(Item *item)
{
    switch (item->type())
    {
        case Item::tBasic:    return new Item(*item);   break;
        case Item::tSound:    return new SoundItem(*dynamic_cast<SoundItem*>(item));    break;
    }
}

Item* ItemFactory::createItem(Item::Type type, const std::string &content, Item::State state)
{
    switch (type)
    {
        case Item::tBasic:    return new Item(content,state);   break;
        case Item::tSound:    return new SoundItem(content,state);    break;
    }
}