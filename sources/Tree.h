#ifndef HEADER_TREE
#define HEADER_TREE

#include "Model.h"
#include <vector>

class Tree;
// alias for a branch
typedef std::pair<Item,Tree> Branch;

class Tree: public Model
{
    private:
        std::vector<Branch*> vChildren;
    public:
        // iterator
        class iterator: public Model::iterator
        {
            private:
                // underlying vector iterators
                std::vector<std::vector<Branch*>::const_iterator> qIts;
            public:
                // constructor
                iterator(const std::vector<std::vector<Branch*>::const_iterator>& its, IterationType type=itNormal, State state=sNone);
                // overloaded operators
                bool operator!=(const iterator& it) const;
                bool operator==(const iterator& it) const;
                iterator& operator++();
                iterator operator++(int i);
                Item operator*();
        };
        // constructor
        Tree(std::string XMLCode = "");
        //destructor
        ~Tree();
        // inherited methods
        std::string toXML() const;
        void fromXML(std::string XMLCode);
        void clear();
        // iterator-related methods
        iterator begin() const;
        iterator beginUnchecked() const;
        iterator beginState(State state) const;
        iterator end() const;        
        iterator endUnchecked() const;
        iterator endState(State state) const;
        // accessors
        Item operator[](std::string indices) const;   // reader
        Item& operator[](std::string indices);        // writer
        Branch* branch(std::string indices);
        // populating the list
        void insert(std::string indices, std::string content, State state);
        void insert(std::string indices, Branch *branch);
        void remove(std::string indices, bool toDelete=true);
        void move(std::string currentIndices, std::string newIndices);
};

#endif
