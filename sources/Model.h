#ifndef HEADER_MODEL
#define HEADER_MODEL

#include <iterator>
#include <string>

// Different possible states for the items of the model   
enum State {sNone,sProgress,sSuccess,sFailure};
// alias for the items
typedef std::pair<std::string,State> Item;

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
        };
        // loading and saving methods
        virtual std::string toXML() const = 0;
        virtual void fromXML(std::string XMLCode) = 0;
        // different iterators
        virtual iterator begin() const = 0;
        virtual iterator beginUnchecked() const = 0;
        virtual iterator beginState(State state) const = 0;
        virtual iterator end() const = 0;        
};

#endif
