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
        Branch(const Branch &branch, Tree *parent=NULL);
        Branch(Item* item, Tree *parent=NULL);
        Branch(Item* item, const Tree &tree, Tree *parent=NULL);
        Branch(Item* item, const xmlpp::Element &root, Tree* parent=NULL);
        // destructor
        ~Branch();
        //accessors
        Item* item() const;
        Tree& tree();
        // copy operator
        Branch& operator=(const Branch &branch);
};

#endif
