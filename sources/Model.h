#ifndef HEADER_MODEL
#define HEADER_MODEL

#include <iterator>
#include <libxml++/libxml++.h>
#include "Item.h"

class Model
{
    public:
        // different possible types of iterating over the items of the model
        enum IterationType {itNormal,itUnchecked,itState};
        // iterator over the items of the model
        class iterator: public std::iterator<std::forward_iterator_tag,Item>
        {
            private:
                IterationType itType;
                State sState;
            public:
                // constructor
                iterator(IterationType type=itNormal, State state=sNone);
                // accessors
                IterationType type() const;
                State state() const;
                void setType(IterationType type);
                // operators
                virtual const Item* operator*() = 0;
        };
        // loading and saving methods
        virtual void toXML(const std::string &fileName) const = 0;
        virtual void toXML(xmlpp::Element &root) const = 0;
        virtual void fromXML(const std::string &fileName) = 0;
        virtual void fromXML(const xmlpp::Element &root) = 0;
        // emptying method
        virtual void clear() = 0;
        // adding method
        virtual void add(const std::string &content, State state=sNone) = 0;
};

#endif
