/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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
#include "IOConfig.h"
#include <stdexcept>

class Branch;

/*!
 * \brief Tree of items
 */
class Tree
{
    private:
        std::vector<Branch*> vChildren;
        // pointer to its parent branch
        Branch* pParent;
        /*!
         * \brief Getter for branches
         * \param indices Indices of the branch
         * \return Branch at the given indices
         * \throw std::out_of_range Thrown when there is no branch at the given indices
         */
        Branch* branch(const std::string &indices) throw(std::out_of_range);
    public:
        // different possible types of iterating over the items of the model
        enum IterationType {itNormal,itUnchecked,itState};
        // iterator
        class iterator: public std::iterator<std::forward_iterator_tag,Item*>
        {
            private:
                IterationType itType;
                Item::State sState;
                //! underlying vector of iterators
                std::vector<std::vector<Branch*>::const_iterator> qIts;
                //! Indices vector
                std::vector<int> vIndices;
            public:
                /*!
                 * \brief Copy constructor
                 * \param its Iterator
                 * \param type Iteration type
                 * \param state State (used for Item::itState iteration type)
                 */
                iterator(const std::vector<std::vector<Branch*>::const_iterator>& its, IterationType type=itNormal, Item::State state=Item::sNone);
                /*!
                 * \brief Constructor from a lower level iterator
                 * \param it Iterator of branches
                 * \param type Iteration type
                 * \param state State (used for Item::itState iteration type)
                 */
                iterator(const std::vector<Branch*>::const_iterator& it, IterationType type=itNormal, Item::State state=Item::sNone);
                // accessors
                IterationType type() const;
                Item::State state() const;
                void setType(IterationType type);
                // overloaded operators
                bool operator!=(const iterator& it) const;
                bool operator==(const iterator& it) const;
                /*!
                 * \brief Incrementation operator
                 * \return The incremented iterator
                 * \throw std::out_of_range Thrown when the iterator has gone beyond the end
                 */
                iterator& operator++() throw(std::out_of_range);
                /*!
                 * \brief Incrementation operator
                 * \param i Dummy argument
                 * \return The iterator before incrementation
                 */
                iterator operator++(int i);
                Item* operator*();
                // information about the position of the iterator
                int depth() const;
                Tree* parent() const;
                Branch* branch() const;
                /*!
                 * \brief Getter for the current indices
                 * \return Indices of the current branch
                 */
                std::string indices() const;
        };
        // constructors
        Tree(Branch* parent=0);
        Tree(const Tree &tree, Branch* parent=0);
        /*!
         * \brief Constructor which read directly in a XML tree
         * \param config IO configuration
         * \param root Root of the XML tree
         * \param checkFiles Indicates if the existence of files associated with FileItems is checked
         * \param parent Pointer to the parent branch of the tree
         */
        Tree(const IOConfig &config, const xmlpp::Element &root, bool checkFiles, Branch* parent);
        // destructor
        ~Tree();
        // copy operator
        Tree& operator=(const Tree &tree);
        // XML-related methods
        /*!
         * \brief XML saver
         * \param config IO configuration
         * \param root Root of the XML tree
         */
        void toXML(const IOConfig &config, xmlpp::Element &root) const;
        /*!
         * \brief XML loader
         * \param config IO configuration
         * \param root Position of the tree in the XML tree
         * \param checkFiles Indicates if the existence of files associated with FileItems is checked
         * \param limitedSize Indicates if the size of the items is limited
         * \throw xmlpp::exception Thrown when an item has not been loaded correctly        
         * \throw std::overflow_error Thrown when the size of a file exceeds the limit (if limited)
         */
        void fromXML(const IOConfig &config, const xmlpp::Element &root, bool checkFiles, bool limitedSize = false) throw(xmlpp::exception, std::overflow_error);
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
        /*!
         * \brief Branch insertion
         * \param indices Indices of where to insert the branch
         * \param branch Branch to insert
         * \throw std::out_of_range Thrown when it is impossible to insert the branch at the given indices
         */
        void insert(const std::string &indices, Branch *branch) throw(std::out_of_range);
        Branch* insert(int index, Item *item) throw(std::out_of_range);
        Branch* insert(const std::string &indices, Item *item);
        /*!
         * \brief Setter for items
         * \param indices Indices of the item to modify
         * \param newItem New item
         * \throw std::out_of_range Thrown when there is no item at the given indices
         */
        void setItem(std::string &indices, Item *newItem) throw(std::out_of_range);
        Branch* add(Item *item);
        void remove(int index, bool toDelete=true) throw(std::out_of_range);
        void remove(const std::string &indices, bool toDelete=true);
        /*!
         * \brief Moves an item
         * \param currentIndices Indices of the item to move
         * \param newIndices Indices of the future emplacement of the item
         * \return True if succeeded, false otherwise
         */
        bool move(const std::string &currentIndices, const std::string &newIndices);
        void clear();
        /*!
         * \brief Index extractor
         * \param indices Indices to extract from
         * \param forward True to begin from the left, false from the right
         * \return Extracted indices
         *
         * After the call, indices contains only indices that have not been extracted yet
         */
        static int extractIndex(std::string &indices, bool forward = true);
        // get the index of a branch
        int indexOf(Branch *branch) const;
        std::string indicesOf(Branch *branch) const throw(std::out_of_range);
        std::string indicesOfNext(Branch *branch) const;
};

#include "Branch.h"

#endif
