/*************************************************************************
* Copyright © 2012-2013 Vincent Prat & Simon Nicolas
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

#include "TreeModification.h"
#include "ItemFactory.h"

using namespace std;

TreeModification::TreeModification(Tree &tree, Item *newItem, const string &indices): Modification(aAddition), sIndices(indices), rTree(tree), pBranch(0), pItem(0), pNewItem(newItem)
{
}

TreeModification::TreeModification(Tree &tree, Branch *branch, const string &indices): Modification(aDeletion), sIndices(indices), rTree(tree), pBranch(branch), pItem(0), pNewItem(0)
{
}

TreeModification::TreeModification(Tree &tree, Item *item, Item *newItem, const string &indices): Modification(aEdition), etEditType(etFull), sIndices(indices), rTree(tree), pBranch(0), pItem(item), pNewItem(newItem)
{
}

TreeModification::TreeModification(Tree &tree, const string &content, const string &newContent, const string &indices): Modification(aEdition), etEditType(etContent), sIndices(indices), rTree(tree), pBranch(0), pItem(0), pNewItem(0), sContent(content), sNewContent(newContent)
{
}

TreeModification::TreeModification(Tree &tree, Item::State state, Item::State newState, const string &indices): Modification(aEdition), etEditType(etState), sIndices(indices), rTree(tree), pBranch(0), pItem(0), pNewItem(0), sState(state), sNewState(newState)
{
}

TreeModification::TreeModification(Tree &tree, const string &indices, const string &newIndices): Modification(aMovement), sIndices(indices), sNewIndices(newIndices), rTree(tree), pBranch(0), pItem(0), pNewItem(0)
{
}

TreeModification::~TreeModification()
{
    if (pBranch)
    {
        delete pBranch;
    }
    if (pItem)
    {
        delete pItem;
    }
    if (pNewItem)
    {
        delete pNewItem;
    }
}

Modification::Type TreeModification::type() const
{
    return tTree;
}

void TreeModification::undo()
{
    switch (action())
    {
        case aAddition: rTree.remove(sIndices);
                        break;
        case aDeletion: rTree.insert(sIndices, new Branch(*pBranch));
                        break;
        case aEdition:  switch (etEditType)
                        {
                            case etFull:    rTree.setItem(sIndices, ItemFactory::copyItem(pItem));
                                            break;
                            case etContent: rTree[sIndices]->setContent(sContent);
                                            break;
                            case etState:   rTree[sIndices]->setState(sState);
                                            break;
                        }
                        break;
        case aMovement: rTree.move(modifiedNewIndices(), modifiedIndices());
                        break; 
        default:    break;
    }
}

void TreeModification::redo()
{
    switch (action())
    {
        case aAddition: rTree.insert(sIndices, new Branch(ItemFactory::copyItem(pNewItem)));
                        break;
        case aDeletion: rTree.remove(sIndices);
                        break;
        case aEdition:  switch (etEditType)
                        {
                            case etFull:    rTree.setItem(sIndices, ItemFactory::copyItem(pNewItem));
                                            break;
                            case etContent: rTree[sIndices]->setContent(sNewContent);
                                            break;
                            case etState:   rTree[sIndices]->setState(sNewState);
                                            break;
                        }
                        break;
        case aMovement: rTree.move(sIndices, sNewIndices);
                        break;
        default:    break;
    }
}

string TreeModification::indices() const
{
    return sIndices;
}

Tree& TreeModification::tree()
{
    return rTree;
}

string TreeModification::modifiedIndices() const
{
    // we now determine if there is need to modify sIndices for the undoing
    string subNew(sNewIndices), subCurrent(sIndices);
    int nNew, nCurrent;
    ostringstream buf;
    // we iterate over the indices
    bool firstTime = true;
    do
    {
        nNew = Tree::extractIndex(subNew);
        nCurrent = Tree::extractIndex(subCurrent);
        
        if (!firstTime)
        {
            buf << "_";
        }

        if (subNew=="" && nNew <= nCurrent)
        {
            // we modify the indices
            buf << (nCurrent+1);
        }
        else
        {
            // we do not modify the indices of the item to remove
            buf << nCurrent;
        }
        firstTime = false;
    }
    while (subNew!="" && subCurrent!="" && nNew == nCurrent);

    // we add the following indices (they are unchanged)
    if (subCurrent != "")
    {
        buf << "_" << subCurrent;
    }
    return buf.str();
}

string TreeModification::modifiedNewIndices() const
{
    // we now determine if there is need to modify sNewIndices for the undoing
    string subNew(sNewIndices), subCurrent(sIndices);
    int nNew, nCurrent;
    ostringstream buf;
    // we iterate over the indices
    bool firstTime = true;
    do
    {
        nNew = Tree::extractIndex(subNew);
        nCurrent = Tree::extractIndex(subCurrent);
        
        if (!firstTime)
        {
            buf << "_";
        }

        if (subCurrent=="" && nCurrent <= nNew)
        {
            // we modify the new indices
            buf << (nNew-1);
        }
        else
        {
            // we do not modify the indices of the item to remove
            buf << nNew;
        }
        firstTime = false;
    }
    while (subNew!="" && subCurrent!="" && nNew == nCurrent);

    // we add the following indices (they are unchanged)
    if (subNew!="")
    {
        buf << "_" << subNew;
    }
    return buf.str();
}

string TreeModification::deletedIndices() const
{
    string sub(sIndices);
    int n;
    ostringstream buf;
    n = Tree::extractIndex(sub, false);
    if (n == 0)
    {
        return sub;
    }
    else
    {
        buf << sub;
        if (sub != "")
        {
            buf << "_";
        }
        buf << n-1;
        return buf.str();
    }
}
