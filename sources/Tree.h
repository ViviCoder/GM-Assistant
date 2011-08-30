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
        // branch accessor
        Branch* branch(const std::string &indices);
        // branch inserter
        void insert(const std::string &indices, Branch *branch);
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
                const Item& operator*();
                // information about the position of the iterator
                int depth() const;

        };
        // constructors
        Tree();
        Tree(const Tree &tree);
        Tree(const xmlpp::Element &root);
        Tree(const std::string &fileName);
        // destructor
        ~Tree();
        // copy operator
        Tree& operator=(const Tree &tree);
        // inherited methods
        void toXML(const std::string &fileName) const;
        void toXML(xmlpp::Element &root) const;
        void fromXML(const std::string &fileName);
        void fromXML(const xmlpp::Element &root);
        void clear();
        // iterator-related methods
        iterator begin() const;
        iterator beginUnchecked() const;
        iterator beginState(State state) const;
        iterator end() const;        
        iterator endUnchecked() const;
        iterator endState(State state) const;
        // accessors
        const Item& operator[](const std::string &indices) const;   // reader
        Item& operator[](const std::string &indices);        // writer
        // populating the list
        void insert(const std::string &indices, const std::string &content, State state=sNone);
        void add(const std::string &content, State state=sNone);
        void add(int depth, const std::string &content, State state=sNone);
        void addChild(const std::string &content, State state=sNone);
        void remove(const std::string &indices, bool toDelete=true);
        void move(const std::string &currentIndices, const std::string &newIndices);
};

#endif
