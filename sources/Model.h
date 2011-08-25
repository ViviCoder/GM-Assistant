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
                void setType(IterationType type);
                // operators
                virtual Item operator*() = 0;
        };
        // loading and saving methods
        virtual std::string toXML() const = 0;
        virtual void fromXML(const std::string &fileName) = 0;
        // emptying method
        virtual void clear() = 0;
        // adding method
        virtual void add(const std::string &content, State state=sNone) = 0;
};

#endif
