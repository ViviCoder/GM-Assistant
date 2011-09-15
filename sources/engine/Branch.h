#ifndef HEADER_BRANCH
#define HEADER_BRANCH

#include "Tree.h"

class Branch
{
    private:
        Item *pItem;
        Tree tTree;
        Branch *pParent;
    public:
        // constructors
        Branch(const Branch &branch, Branch *parent=NULL);
        Branch(Item* item, Branch *parent=NULL);
        Branch(Item* item, const Tree &tree, Branch *parent=NULL);
        Branch(Item* item, const xmlpp::Element &root, Branch* parent=NULL);
        // destructor
        ~Branch();
        //accessors
        Item* item() const;
        Tree& tree();
        Branch* parent() const;
        // copy operator
        Branch& operator=(const Branch &branch);
};

#endif
