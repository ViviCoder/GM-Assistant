#include "List.h"

using namespace std;

// constructor

List::List(const string &fileName): data()
{
    fromXML(fileName);
}

// inherited methods

string List::toXML() const
{
    // not yet implemented
    return "";
}

void List::fromXML(const string &fileName)
{
    // not yet implemented
}

void List::clear()
{
    data.clear();
}

// new methods

List::iterator List::begin() const
{
    return iterator(data.begin());
}

List::iterator List::beginUnchecked() const
{
    vector<Item>::const_iterator it = data.begin();
    while (it != data.end() && (it->second==sSuccess || it->second==sFailure))
        it++;
    return iterator(it,itUnchecked);
}

List::iterator List::beginState(State state) const
{
    vector<Item>::const_iterator it = data.begin();
    while (it != data.end() && (it->second!=state))
        it++;
    return iterator(it,itState,state);
}

List::iterator List::end() const
{
    return iterator(data.end());
}

List::iterator List::endUnchecked() const
{
    vector<Item>::const_iterator it = data.end();
    if (beginUnchecked()!=end())
    {
        it--;
        while (it->second==sSuccess || it->second==sFailure)
            it--;
        it++;
    }
    return iterator(it);
}

List::iterator List::endState(State state) const
{
    vector<Item>::const_iterator it = data.end();
    if (beginState(state)!=end())
    {
        it--;
        while (it->second!=state)
            it--;
        it++;
    }
    return iterator(it);
}

pair<string,State> List::operator[](int index) const
{
    if (index<0 || (unsigned int)index >= data.size())
    {
        throw string("List::operator[] : Index out of bounds");
    }
    return data[index];
}

pair<string,State>& List::operator[](int index)
{
    if (index<0 || (unsigned int)index >= data.size())
    {
        throw string("List::operator[] : Index out of bounds");
    }
    return data[index];
}

void List::insert(int index, const string &content, State state)
{
    if (index<0 || (unsigned int)index > data.size()) // index can be data.size() 
    {
        throw string("List::insert : Index out of bounds");
    }
    data.insert(data.begin()+index,Item(content,state));
}

void List::remove(int index)
{
    if (index<0 || (unsigned int)index >= data.size()) // index can be data.size() 
    {
        throw string("List::remove : Index out of bounds");
    }
    data.erase(data.begin()+index);
}

void List::move(int currentIndex, int newIndex)
{
    // the new index is counted before the move
    if (currentIndex<0 || newIndex<0 || (unsigned int)currentIndex >= data.size() || (unsigned int)newIndex >= data.size()) // index can be data.size() 
    {
        throw string("List::move : Index out of bounds");
    }
    // test if the indices are the same
    if (currentIndex==newIndex) return;
    data.insert(data.begin()+newIndex,operator[](currentIndex));
    if (currentIndex>newIndex)
    {
        data.erase(data.begin()+currentIndex+1);
    }
    else
    {
        data.erase(data.begin()+currentIndex);
    }
}

// iterator's methods

List::iterator::iterator(const vector<Item>::const_iterator& it, IterationType type, State state): Model::iterator(type,state), viIt(it)
{
}

bool List::iterator::operator!=(const iterator& it) const
{
    return it.viIt!=viIt;
}

bool List::iterator::operator==(const iterator& it) const
{
    return it.viIt==viIt;
}

List::iterator& List::iterator::operator++()
{
    viIt++;
    return *this;
}

List::iterator List::iterator::operator++(int i)
{
    iterator it = *this;
    operator++(); 
    return it;
}

Item List::iterator::operator*()
{
    // It is at this moment that you have to go to the next unchecked item or to the next item of the given state 
    switch (type())
    {
        case itUnchecked:   while (viIt->second==sSuccess || viIt->second==sFailure)
                                operator++();
                            break;
        case itState:
            {
                State sState = state();
                while (viIt->second!=sState)
                    operator++();
                break;
            }
        default:            break;
    }
    return *viIt;
}
