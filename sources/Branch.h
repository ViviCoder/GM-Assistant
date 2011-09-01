#ifndef HEADER_BRANCH
#define HEADER_BRANCH

#include "Tree.h"

class Branch
{
    private:
        Item *pItem;
        Tree tTree;
    public:
        // constructors
        Branch(const Branch &branch);
        Branch(Item* item, const Tree &tree);
        // destructor
        ~Branch();
        //accessors
        Item* item() const;
        Tree& tree();
        // copy operator
        Branch& operator=(const Branch &branch);
};

#endif
