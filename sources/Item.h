#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>

// Different possible states for the item   
enum State {sNone,sProgress,sSuccess,sFailure};

class Item
{
    private:
        State sState;
        std::string sContent;
    public:
        // constructor
        Item(const std::string &content="", State state=sNone);
        // accessors
        const std::string& content() const;
        void setContent(const std::string &content);
        State state() const;
        void setState(State state);
};

#endif
