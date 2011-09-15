#include "Item.h"

using namespace std;

Item::Item(const string &content, State state): sState(state), sContent(content)
{
}

string Item::content() const
{
    return sContent;
}

void Item::setContent(const string &content)
{
    sContent = content;
}

Item::State Item::state() const
{
    return sState;
}

void Item::setState(State state)
{
    sState = state;
}

Item::Type Item::type() const
{
    return tBasic;
}

void Item::fromXML(const xmlpp::Element &root)
{
}

void Item::toXML(xmlpp::Element &root)
{
}

string Item::stateToStr(State state)
{
    switch (state)
    {
        case sNone: return "none"; break;
        case sProgress: return "progress";  break;
        case sFailure:  return "failure";   break;
        case sSuccess:  return "success";   break;
    }
} 

string Item::typeToStr(Type type)
{
    switch (type)
    {
        case tBasic:    return "basic"; break;
        case tSound:    return "sound"; break;
    }
}

Item::State Item::strToState(const string &name) throw(invalid_argument)
{
    if (name=="none")   return sNone;
    else if (name=="progress")  return sProgress;
    else if (name=="failure")   return sFailure;
    else if (name=="success")   return sSuccess;
    else    throw invalid_argument("Unrecognized \""+name+"\" item state");
}

Item::Type Item::strToType(const string &name) throw(invalid_argument)
{
    if (name=="basic")  return tBasic;
    else if (name=="sound") return tSound;
    else    throw invalid_argument("Unrecognized \""+name+"\" item type");
}
