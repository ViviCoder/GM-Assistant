#ifndef HEADER_LIST
#define HEADER_LIST

#include "Model.h"
#include <vector>

class List: public Model
{
    private:
        // data contained in the list
        std::vector<Item> data;
    public:
        // iterator
        class iterator: public Model::iterator
        {            
            private:
                // underlying vector iterator
                std::vector<Item>::const_iterator viIt;
            public:
                // constructors
                iterator(std::vector<Item>::const_iterator it, IterationType type=itNormal, State state=sNone);
        };
        // constructor
        List(std::string XMLCode);
        // inherited methods
        std::string toXML() const;
        void fromXML(std::string XMLCode);
        Model::iterator begin() const;
        Model::iterator beginUnchecked() const;
        Model::iterator beginState(State state) const;
        Model::iterator end() const;        
        // accessors
        Item operator[](int index) const;   // reader
        Item& operator[](int index);        // writer
        // populating the list
        void insert(int index, std::string content, State state);
        void remove(int index);
        void move(int currentIndex, int newIndex);
};

#endif
