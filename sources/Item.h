#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>

class Item
{
    public:
        // Different possible states for the item   
        enum State {sNone,sProgress,sSuccess,sFailure};
        // Different possible type for the item   
        enum Type {tBasic, tSound};
        // constructor
        Item(const std::string &content="", State state=sNone);
        // accessors
        std::string content() const;
        void setContent(const std::string &content);
        State state() const;
        void setState(State state);
        virtual Type type() const;
    private:
        State sState;
        std::string sContent;
};

#endif
