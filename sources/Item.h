#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>

class Item
{
    public:
        // Different possible states for the item   
        enum State {sNone,sProgress,sSuccess,sFailure};
        // constructor
        Item(const std::string &content="", State state=sNone);
        // accessors
        std::string content() const;
        void setContent(const std::string &content);
        State state() const;
        void setState(State state);
    private:
        State sState;
        std::string sContent;
};

#endif
