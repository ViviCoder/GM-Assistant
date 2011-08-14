#include "List.h"

// inherited methods

std::string List::toXML() const
{
    // not yet implemented
    return "";
}

void List::fromXML(std::string XMLCode)
{
    // not yet implemented
}

List::iterator List::begin() const
{
    return List::iterator(data.begin());
}

List::iterator List::beginUnchecked() const
{
    std::vector<Item>::const_iterator it = data.begin();
    while (it != data.end() && (it->second==sSuccess || it->second==sFailure)) it++;
    return List::iterator(it,itUnchecked);
}

List::iterator List::beginState(State state) const
{
    std::vector<Item>::const_iterator it = data.begin();
    while (it != data.end() && (it->second!=state)) it++;
    return List::iterator(it,itState,state);
}

// new methods

List::iterator List::end() const
{
    return List::iterator(data.end());
}

List::List(std::string XMLCode): data()
{
    fromXML(XMLCode);
}

std::pair<std::string,State> List::operator[](int index) const
{
    if (index<0 || (unsigned int)index >= data.size())
    {
        throw std::string("Index out of range");
    }
    return data[index];
}

std::pair<std::string,State>& List::operator[](int index)
{
    if (index<0 || (unsigned int)index >= data.size())
    {
        throw std::string("Index out of range");
    }
    return data[index];
}

void List::insert(int index, std::string content, State state)
{
    if (index<0 || (unsigned int)index > data.size()) // index can be data.size() 
    {
        throw std::string("Index out of range");
    }
    data.insert(data.begin()+index,Item(content,state));
}

void List::remove(int index)
{
    if (index<0 || (unsigned int)index >= data.size()) // index can be data.size() 
    {
        throw std::string("Index out of range");
    }
    data.erase(data.begin()+index);
}

void List::move(int currentIndex, int newIndex)
{
    // the new index is counted before the move
    if (currentIndex<0 || newIndex<0 || (unsigned int)currentIndex >= data.size() || (unsigned int)newIndex >= data.size()) // index can be data.size() 
    {
        throw std::string("Index out of range");
    }
    // test if the indices are the same
    if (currentIndex==newIndex) return;
    Item item = data[currentIndex];
    data.erase(data.begin()+currentIndex);
    if (currentIndex>newIndex)
    {
        data.insert(data.begin()+newIndex,item);
    }
    else
    {
        data.insert(data.begin()+newIndex-1,item);
    }
}

// iterator's methods
List::iterator::iterator(const std::vector<Item>::const_iterator& it, IterationType type, State state): Model::iterator(type,state), viIt(it)
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
    List::iterator it = *this;
    operator++(); 
    return it;
}

Item List::iterator::operator*() const
{
    return *viIt;
}
