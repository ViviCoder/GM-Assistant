#ifndef HEADER_TREE
#define HEADER_TREE

#include "Item.h"
#include <vector>
#include <iterator>

class Branch;

class Tree
{
    private:
        std::vector<Branch*> vChildren;
        // pointer to its parent branch
        Branch* pParent;
        // branch accessor
        Branch* branch(const std::string &indices) throw(std::out_of_range);
        // branch inserter
        void insert(const std::string &indices, Branch *branch) throw(std::out_of_range);
    public:
        // different possible types of iterating over the items of the model
        enum IterationType {itNormal,itUnchecked,itState};
        // iterator
        class iterator: public std::iterator<std::forward_iterator_tag,Item*>
        {
            private:
                IterationType itType;
                Item::State sState;
                // underlying vector iterators
                std::vector<std::vector<Branch*>::const_iterator> qIts;
            public:
                // constructor
                iterator(const std::vector<std::vector<Branch*>::const_iterator>& its, IterationType type=itNormal, Item::State state=Item::sNone);
                iterator(const std::vector<Branch*>::const_iterator& it, IterationType type=itNormal, Item::State state=Item::sNone);
                // accessors
                IterationType type() const;
                Item::State state() const;
                void setType(IterationType type);
                // overloaded operators
                bool operator!=(const iterator& it) const;
                bool operator==(const iterator& it) const;
                iterator& operator++() throw(std::out_of_range);
                iterator operator++(int i);
                Item* operator*();
                // information about the position of the iterator
                int depth() const;
                Branch* parent() const;
                Branch* branch() const;
        };
        // constructors
        Tree(Branch* parent=NULL);
        Tree(const Tree &tree, Branch* parent=NULL);
        Tree(const xmlpp::Element &root, Branch* parent=NULL);
        Tree(const std::string &fileName) throw(xmlpp::exception);
        // destructor
        ~Tree();
        // copy operator
        Tree& operator=(const Tree &tree);
        // inherited methods
        void toXML(const std::string &fileName) const;
        void toXML(xmlpp::Element &root) const;
        void fromXML(const std::string &fileName) throw(xmlpp::exception);
        void fromXML(const xmlpp::Element &root);
        void clear();
        // iterator-related methods
        iterator begin() const;
        iterator beginUnchecked() const;
        iterator beginState(Item::State state) const;
        iterator end() const;        
        iterator endUnchecked() const;
        iterator endState(Item::State state) const;
        // accessors
        Item* operator[](const std::string &indices) throw(std::out_of_range);
        Branch* parent() const;
        // populating the list
        Branch* insert(int index, Item *item) throw(std::out_of_range);
        Branch* insert(const std::string &indices, Item *item);
        Branch* add(Item *item);
        Branch* add(int depth, Item *item) throw(std::out_of_range);
        Branch* addChild(Item *item);
        void remove(int index, bool toDelete=true) throw(std::out_of_range);
        void remove(const std::string &indices, bool toDelete=true);
        void move(const std::string &currentIndices, const std::string &newIndices);
        // index extractor
        static int extractIndex(std::string &indices);
        // get the index of a branch
        int indexOf(Branch *branch) const;
};

#include "Branch.h"

#endif
