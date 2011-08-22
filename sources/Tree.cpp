#include "Tree.h"

// constructor

Tree::Tree(std::string XMLCode): vChildren()
{
    fromXML(XMLCode);
}

// destructor

Tree::~Tree()
{
    for (std::vector<Branch*>::iterator it = vChildren.begin(); it != vChildren.end(); it++)
    {
        delete *it;
    }
}

//inherited methods

std::string Tree::toXML() const
{
    // not yet implemented
    return "";
}

void Tree::fromXML(std::string XMLCode)
{
    // not yet implemented
}

void Tree::clear()
{
    // not complete
    vChildren.clear();
}

// new methods

Tree::iterator Tree::begin() const
{
    std::vector<std::vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    return iterator(stack);
}

Tree::iterator Tree::beginUnchecked() const
{
    std::vector<std::vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    iterator it(stack);
    while (it != end() && ((*it).second==sSuccess || (*it).second==sFailure))
    {
        it++;
    }
    it.setType(itUnchecked);
    return it;
}

Tree::iterator Tree::beginState(State state) const
{
    std::vector<std::vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    iterator it(stack,itNormal,state);
    while (it != end() && (*it).second!=state)
    {
        it++;
    }
    it.setType(itState);
    return it;
}

Tree::iterator Tree::end() const
{
    std::vector<std::vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.end());
    return iterator(stack);
}

Tree::iterator Tree::endUnchecked() const
{
    iterator it=beginUnchecked(),it2=it;
    it.setType(itNormal);
    while (it!=end())
    {
        it++;
        if ((*it).second==sNone || (*it).second==sProgress)
        {
            it2 = it;    
        }
    }
    return it2;
}

Tree::iterator Tree::endState(State state) const
{
    iterator it=beginState(state),it2=it;
    it.setType(itNormal);
    while (it!=end())
    {
        it++;
        if ((*it).second==state)
        {
            it2 = it;    
        }
    }
    return it2;
}

// iterator's methods

Tree::iterator::iterator(const std::vector<std::vector<Branch*>::const_iterator>& its, IterationType type, State state): Model::iterator(type,state), qIts(its)
{
}

bool Tree::iterator::operator!=(const iterator& it) const
{
    if (it.qIts.size()!=qIts.size())
    {
        return true;
    }
    std::vector<std::vector<Branch*>::const_iterator>:: const_iterator it2 = it.qIts.begin();
    for (std::vector<std::vector<Branch*>::const_iterator>:: const_iterator it1 = qIts.begin(); it1 !=qIts.end(); it1++)
    {
        if (*it1 != *it2)
        {
            return true;
        }
        it2++;
    }
    return false;
}

bool Tree::iterator::operator==(const iterator& it) const
{
    return !operator!=(it);
}

Tree::iterator& Tree::iterator::operator++()
{
    std::vector<std::vector<Branch*>::const_iterator>::reverse_iterator it = qIts.rbegin();
    if (it==qIts.rend())
    {
        throw std::string("End of the tree reached");
    }
    if ((*(*it))->second.vChildren.size()!=0)   // if there are children, go to the first child
    {
        qIts.push_back((*(*it))->second.vChildren.begin());
    }
    else    // otherwise, go to the next (if there is one)
    {
        (*it)++;
        while (it-1 != qIts.rend() && *it == (*(*(it-1)))->second.vChildren.end())
        {
            qIts.pop_back();
        }
    }   
    return *this;
}

Tree::iterator Tree::iterator::operator++(int i)
{
    Tree::iterator it = *this;
    operator++();
    return it;
}

Item Tree::iterator::operator*()
{
    switch (type())
    {
        case itUnchecked:   while ((*(qIts.back()))->first.second==sSuccess || (*(qIts.back()))->first.second==sFailure)
                                operator++();
                            break; 
        case itState:
            {
                State sState = state();
                while ((*(qIts.back()))->first.second!=sState)
                    operator++();
                break;
            }
        default:            break;
    }
    return (*(qIts.back()))->first;
}
