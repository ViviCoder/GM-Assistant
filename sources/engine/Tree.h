/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_TREE
#define HEADER_TREE

#include "Item.h"
#include <vector>
#include <iterator>

class Branch;

/*!
 * \brief Tree
 */
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
                Tree* parent() const;
                Branch* branch() const;
        };
        // constructors
        Tree(Branch* parent=NULL);
        Tree(const Tree &tree, Branch* parent=NULL);
        /*!
         * \brief Constructor which read directly in a XML tree
         * \param root Root of the XML tree
         * \param checkFiles Indicates if the existence of files associated with FileItems is checked
         * \param parent Pointer to the parent branch of the tree
         */
        Tree(const xmlpp::Element &root, bool checkFiles, Branch* parent);
        // destructor
        ~Tree();
        // copy operator
        Tree& operator=(const Tree &tree);
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        /*!
         * \brief XML loader
         * \param root Position of the tree in the XML tree
         * \param checkFiles Indicates if the existence of files associated with FileItems is checked
         * \param limitedSize Indicates if the size of the items is limited
         * \throw std::invalid_argument Thrown when an item has not been loaded correctly        
         * \throw std::overflow_error Thrown when the size of a file exceeds the limit (if limited)

         */
        void fromXML(const xmlpp::Element &root, bool checkFiles, bool limitedSize = false) throw(std::invalid_argument, std::overflow_error);
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
        unsigned int numberOfChildren() const;
        // populating the list
        Branch* insert(int index, Item *item) throw(std::out_of_range);
        Branch* insert(const std::string &indices, Item *item);
        Branch* add(Item *item);
        void remove(int index, bool toDelete=true) throw(std::out_of_range);
        void remove(const std::string &indices, bool toDelete=true);
        void move(const std::string &currentIndices, const std::string &newIndices);
        void clear();
        // index extractor
        static int extractIndex(std::string &indices);
        // get the index of a branch
        int indexOf(Branch *branch) const;
        std::string indicesOf(Branch *branch) const throw(std::out_of_range);
        std::string indicesOfNext(Branch *branch) const;
};

#include "Branch.h"

#endif
