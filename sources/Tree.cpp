#include "Tree.h"
#include <cstdlib>

using namespace std;

// constructor

Tree::Tree(string XMLCode): vChildren()
{
    fromXML(XMLCode);
}

// destructor

Tree::~Tree()
{
    clear();
}

//inherited methods

string Tree::toXML() const
{
    // not yet implemented
    return "";
}

void Tree::fromXML(string XMLCode)
{
    // not yet implemented
}

void Tree::clear()
{
    for (vector<Branch*>::iterator it = vChildren.begin(); it != vChildren.end(); it++)
    {
        (*it)->second.clear();
        delete *it;
    }
}

// new methods

Tree::iterator Tree::begin() const
{
    vector<vector<Branch*>::const_iterator> stack;
    stack.push_back(vChildren.begin());
    return iterator(stack);
}

Tree::iterator Tree::beginUnchecked() const
{
    vector<vector<Branch*>::const_iterator> stack;
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
    vector<vector<Branch*>::const_iterator> stack;
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
    vector<vector<Branch*>::const_iterator> stack;
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

Item Tree::operator[](string indices) const
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        return vChildren[n]->first;
    }
    else
    {
        return vChildren[n]->second[sub];
    }
}

Item& Tree::operator[](string indices)
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        return vChildren[n]->first;
    }
    else
    {
        return vChildren[n]->second[sub];
    }
}

Branch* Tree::branch(string indices)
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        return vChildren[n];
    }
    else
    {
        return vChildren[n]->second.branch(sub);
    }
}

void Tree::insert(string indices, string content, State state)
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        Branch *branch = new Branch;
        branch->first = pair<string,State>(content,state);
        vChildren.insert(vChildren.begin()+n,branch);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw string("Tree::operator[] : Index out of bounds");
        }
        vChildren[n]->second.insert(sub,content,state);
    }
}

void Tree::insert(string indices, Branch *branch)
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        vChildren.insert(vChildren.begin()+n,branch);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw string("Tree::operator[] : Index out of bounds");
        }
        vChildren[n]->second.insert(sub,branch);
    }
}

void Tree::remove(string indices, bool toDelete)
{
    int pos = indices.find("_");
    int n = atoi(indices.substr(0,pos).c_str());
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw string("Tree::operator[] : Index out of bounds");
    }
    string sub = indices.substr(pos+1);
    if (sub=="")
    {
        if (toDelete)
        {
            vChildren[n]->second.clear();
        }
        vChildren.erase(vChildren.begin()+n);
    }
    else
    {
        vChildren[n]->second.remove(sub);
    }
}

void Tree::move(string currentIndices, string newIndices)
{
    insert(newIndices,branch(currentIndices));
    remove(currentIndices,false);    
}

// iterator's methods

Tree::iterator::iterator(const vector<vector<Branch*>::const_iterator>& its, IterationType type, State state): Model::iterator(type,state), qIts(its)
{
}

bool Tree::iterator::operator!=(const iterator& it) const
{
    if (it.qIts.size()!=qIts.size())
    {
        return true;
    }
    vector<vector<Branch*>::const_iterator>:: const_iterator it2 = it.qIts.begin();
    for (vector<vector<Branch*>::const_iterator>:: const_iterator it1 = qIts.begin(); it1 !=qIts.end(); it1++)
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
    vector<vector<Branch*>::const_iterator>::reverse_iterator it = qIts.rbegin();
    if (it==qIts.rend())
    {
        throw string("Tree::iterator::operator++ : End of the tree reached");
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
