#include "List.h"

using namespace std;

// constructors

List::List()
{
}

List::List(const string &fileName)
{
    fromXML(fileName);
}

// destructor

List::~List()
{
    clear();
}

// inherited methods

void List::toXML(const string &fileName) const
{
    using namespace xmlpp;
    
    Document document;
    Element *root = document.create_root_node("list");
    toXML(*root);
    document.write_to_file_formatted(fileName,"UTF-8");
}

void List::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;
    
    for (vector<Item*>::const_iterator it=data.begin(); it!=data.end(); it++)
    {
        Element *tmp = root.add_child("item");
        stringstream buf(stringstream::in | stringstream::out);
        buf << (*it)->state();
        tmp->set_attribute("state",buf.str());
        tmp->set_child_text((*it)->content());
    }
}

void List::fromXML(const string &fileName)
{
    using namespace xmlpp;
    
    DomParser parser(fileName);
    Document *document = parser.get_document();    
    Element *root = document->get_root_node();
    if (root->get_name()!="list")
    {
        throw string("List::fromXML : the given XML file doesn't contain a list");
    }
    fromXML(*root);
}

void List::fromXML(const xmlpp::Element &root)
{
    clear();
    using namespace xmlpp;
    
    Node::NodeList list = root.get_children("item");
    for (Node::NodeList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        Attribute *attr = elem->get_attribute("state");
        int state = 0;
        if (attr!=NULL)
        {
            stringstream buf(stringstream::in | stringstream::out);
            buf << attr->get_value();
            buf >> state;
        }
        data.push_back(new Item(elem->get_child_text()->get_content(),State(state)));
    }
}

void List::clear()
{
    for (vector<Item*>::iterator it = data.begin(); it != data.end(); it++)
    {
        delete (*it);
    }
    data.clear();
}

// new methods

List::iterator List::begin() const
{
    return iterator(data.begin());
}

List::iterator List::beginUnchecked() const
{
    vector<Item*>::const_iterator it = data.begin();
    while (it != data.end() && ((*it)->state()==sSuccess || (*it)->state()==sFailure))
        it++;
    return iterator(it,itUnchecked);
}

List::iterator List::beginState(State state) const
{
    vector<Item*>::const_iterator it = data.begin();
    while (it != data.end() && ((*it)->state()!=state))
        it++;
    return iterator(it,itState,state);
}

List::iterator List::end() const
{
    return iterator(data.end());
}

List::iterator List::endUnchecked() const
{
    vector<Item*>::const_iterator it = data.end();
    if (beginUnchecked()!=end())
    {
        it--;
        while ((*it)->state()==sSuccess || (*it)->state()==sFailure)
            it--;
        it++;
    }
    return iterator(it);
}

List::iterator List::endState(State state) const
{
    vector<Item*>::const_iterator it = data.end();
    if (beginState(state)!=end())
    {
        it--;
        while ((*it)->state()!=state)
            it--;
        it++;
    }
    return iterator(it);
}

Item* List::operator[](int index)
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
    data.insert(data.begin()+index,new Item(content,state));
}

void List::add(const string &content, State state)
{
    data.push_back(new Item(content,state));
}

void List::remove(int index)
{
    if (index<0 || (unsigned int)index >= data.size()) // index can be data.size() 
    {
        throw string("List::remove : Index out of bounds");
    }
    delete data[index];
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

List::iterator::iterator(const vector<Item*>::const_iterator& it, IterationType type, State state): Model::iterator(type,state), viIt(it)
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

const Item* List::iterator::operator*()
{
    // It is at this moment that you have to go to the next unchecked item or to the next item of the given state 
    switch (type())
    {
        case itUnchecked:   while ((*viIt)->state()==sSuccess || (*viIt)->state()==sFailure)
                                operator++();
                            break;
        case itState:
            {
                State sState = state();
                while ((*viIt)->state()!=sState)
                    operator++();
                break;
            }
        default:            break;
    }
    return *viIt;
}
