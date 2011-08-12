#ifndef HEADER_MODEL
#define HEADER_MODEL

#include <iterator>
#include <string>

// Different possible states for the items of the model   
enum State {sNone,sProgress,sSuccess,sFailure};

class Model
{
    public:
        // different possible types of iterating over the items of the model
        enum IterationType {itNormal,itUnchecked,itState};
        // iterator over the items of the model
        class iterator: public std::iterator<std::random_access_iterator_tag,std::pair<std::string,State> >
        {
            private:
                IterationType itType;
                State sState;
            public:
                // constructor of the iterator
                iterator(IterationType type=itNormal, State state=sNone);
                // accessors
                IterationType type() const;
                State state() const;
        };
        // loading and saving methods
        virtual std::string toXML() const;
        virtual void fromXML(std::string XMLCode);
        // different iterators
        virtual iterator begin() const;
        virtual iterator beginUnchecked() const;
        virtual iterator beginState(State state) const;
        virtual iterator end() const;        
};

#endif
