#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>
#include <stdexcept>
#include <libxml++/libxml++.h>

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
        // XML-related methods
        virtual void fromXML(const xmlpp::Element &root);
        virtual void toXML(xmlpp::Element &root);
        // static methods to get a link between strings and state or type
        static std::string stateToStr(State state);
        static std::string typeToStr(Type type);
        static State strToState(const std::string &name) throw(std::invalid_argument);
        static Type strToType(const std::string &name) throw(std::invalid_argument);
    private:
        State sState;
        std::string sContent;
};

#endif
