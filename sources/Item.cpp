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
