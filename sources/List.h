#ifndef HEADER_LIST
#define HEADER_LIST

#include "Model.h"
#include <vector>
#include <stdexcept>

class List: public Model
{
    private:
        // data contained in the list
        std::vector<Item*> data;
    public:
        // iterator
        class iterator: public Model::iterator
        {            
            private:
                // underlying vector iterator
                std::vector<Item*>::const_iterator viIt;
            public:
                // constructor
                iterator(const std::vector<Item*>::const_iterator& it, IterationType type=itNormal, State state=sNone);
                // overloaded operators
                bool operator!=(const iterator& it) const;
                bool operator==(const iterator& it) const;
                iterator& operator++();
                iterator operator++(int i);
                const Item* operator*();
        };
        // constructors
        List();
        List(const std::string &fileName) throw(xmlpp::exception);
        // destructor
        ~List();
        // inherited methods
        void toXML(const std::string &fileName) const;
        void toXML(xmlpp::Element &root) const;
        void fromXML(const std::string &fileName) throw(xmlpp::exception);
        void fromXML(const xmlpp::Element &root);
        void clear();
        // iterator-related methods
        iterator begin() const;
        iterator beginUnchecked() const;
        iterator beginState(State state) const;
        iterator end() const;        
        iterator endUnchecked() const;
        iterator endState(State state) const;
        // accessor
        Item* operator[](int index) throw(std::out_of_range);
        // populating the list
        void insert(int index, const std::string &content, State state=sNone) throw(std::out_of_range);
        void add(const std::string &content, State state=sNone);
        void remove(int index) throw(std::out_of_range);
        void move(int currentIndex, int newIndex) throw(std::out_of_range);
};

#endif
