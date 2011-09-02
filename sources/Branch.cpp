#include "Branch.h"

// constructors

Branch::Branch(Item* item, Tree *parent): pItem(item), tTree(parent)
{
}

Branch::Branch(Item* item, const Tree &tree, Tree* parent): pItem(item), tTree(tree,parent)
{
}

Branch::Branch(Item* item, const xmlpp::Element &root, Tree* parent): pItem(item), tTree(root,parent)
{
}

Branch::Branch(const Branch &branch, Tree* parent): pItem(new Item(*branch.pItem)),tTree(branch.tTree,parent)
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
