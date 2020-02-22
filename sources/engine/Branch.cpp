/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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

#include "Branch.h"
#include "ItemFactory.h"

// constructors

Branch::Branch(Item* item, Tree *parent): pItem(item), tTree(this), pParent(parent)
{
}

Branch::Branch(Item* item, const Tree &tree, Tree* parent): pItem(item), tTree(tree,this), pParent(parent)
{
}

Branch::Branch(Item* item, const IOConfig &config, const Poco::XML::Element *root, bool checkFiles, Tree* parent): pItem(item), tTree(config, root, checkFiles,this), pParent(parent)
{
}

Branch::Branch(const Branch &branch, Tree* parent): pParent(parent)
{
    *this = branch;
}

// destructor

Branch::~Branch()
{
    delete pItem;
}

// operators

Branch& Branch::operator=(const Branch &branch)
{
    pItem = ItemFactory::copyItem(branch.item());
    tTree = branch.tTree;

    return *this;
}
