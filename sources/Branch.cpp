#include "Branch.h"

// constructor

Branch::Branch(Item* item, const Tree &tree): pItem(item), tTree(tree)
{
}

Branch::Branch(const Branch &branch): pItem(new Item(*branch.pItem)),tTree(branch.tTree)
{
}

// destructor

Branch::~Branch()
{
    delete pItem;
}

//methods

Item* Branch::item() const
{
    return pItem;
}

Tree& Branch::tree()
{
    return tTree;
}

// operators

Branch& Branch::operator=(const Branch &branch)
{
    pItem = new Item(*branch.pItem);
    tTree = branch.tTree;

    return *this;
}
