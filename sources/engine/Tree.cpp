/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include "Tree.h"
#include <sstream>
#include "ItemFactory.h"
#include <Poco/DOM/NodeList.h>
#include <Poco/DOM/Document.h>

using namespace std;

// constructors

Tree::Tree(Branch* parent): pParent(parent)
{
}

Tree::Tree(const Tree &tree, Branch* parent)
{
    *this = tree;
    pParent = parent;
}

Tree::Tree(const IOConfig &config, const Poco::XML::Element *root, bool checkFiles, Branch* parent): pParent(parent)
{
    fromXML(config, root, checkFiles);
}

// destructor

Tree::~Tree()
{
    clear();
}

// copy operator

Tree& Tree::operator=(const Tree &tree)
{
    clear();
    for (vector<Branch*>::const_iterator it=tree.vChildren.begin(); it != tree.vChildren.end(); it++)
    {
        Branch *branch = new Branch(**it,this);
        vChildren.push_back(branch);
    }
    pParent = 0;
    return *this;
}

// methods

void Tree::toXML(const IOConfig &config, Poco::XML::Element *root, FileMapping &fileMapping) const
{
    using namespace Poco::XML;

    Document *document = root->ownerDocument();
    for (vector<Branch*>::const_iterator it=vChildren.begin(); it != vChildren.end(); it++)
    {
        Element *tmp = document->createElement("item");
        root->appendChild(tmp);
        Item *item = (*it)->item();
        tmp->setAttribute("state", Item::stateToStr(item->state()));
        tmp->setAttribute("type", Item::typeToStr(item->type(), config));
        tmp->setAttribute("content", item->content());
        if (config.hasExpanded())
        {
            tmp->setAttribute("expanded", Item::boolToStr((*it)->item()->expanded()));
        }
        if (item->type() != Item::tImage || config.hasImages())
        {
            item->toXML(config, tmp, fileMapping);
        }
        (*it)->tree().toXML(config, tmp, fileMapping);
    }
}

void Tree::fromXML(const IOConfig &config, const Poco::XML::Element *root, bool checkFiles)
{
    clear();
    using namespace Poco::XML;

    NodeList *list = root->childNodes();
    for (int i = 0; i < list->length(); i++)
    {
        Node *node = list->item(i);
        if (node->nodeType() == Node::ELEMENT_NODE)
        {
            Element *elem = static_cast<Element*>(node);
            if (elem->tagName() == "item")
            {
                string attr = elem->getAttribute("state");
                Item::State state = Item::sNone;
                if (!attr.empty())
                {
                    state = Item::strToState(attr);
                }
                attr = elem->getAttribute("type");
                Item::Type type = Item::tBasic;
                if (!attr.empty())
                {
                    type = Item::strToType(attr, config);
                }
                string content = elem->getAttribute("content");
                bool expanded = false;
                if (config.hasExpanded())
                {
                    attr = elem->getAttribute("expanded");
                    if (!attr.empty())
                    {
                        expanded = Item::strToBool(attr);
                    }
                }
                Item *item = ItemFactory::createItem(type,content,state,expanded);
                item->fromXML(config, elem, checkFiles);
                Branch *branch = new Branch(item, config, elem, checkFiles, this);
                vChildren.push_back(branch);
            }
        }
    }
    list->release();
}

void Tree::clear()
{
    for (vector<Branch*>::const_iterator it=vChildren.begin(); it!=vChildren.end(); it++)
    {
        delete *it;
    }
    vChildren.clear();
}

Tree::iterator Tree::begin() const
{
    return iterator(vChildren.begin());
}

Tree::iterator Tree::beginUnchecked() const
{
    iterator it(vChildren.begin());
    while (it != end() && ((*it)->state()==Item::sSuccess || (*it)->state()==Item::sFailure))
    {
        it++;
    }
    it.setType(itUnchecked);
    return it;
}

Tree::iterator Tree::beginState(Item::State state) const
{
    iterator it(vChildren.begin(),itNormal,state);
    while (it != end() && (*it)->state()!=state)
    {
        it++;
    }
    it.setType(itState);
    return it;
}

Tree::iterator Tree::end() const
{
    return iterator(vChildren.end());
}

Tree::iterator Tree::endUnchecked() const
{
    iterator it=beginUnchecked(),it2=it;
    if (it==end())
    {
        return it;
    }
    it.setType(itNormal);
    while (it!=end())
    {
        if ((*it)->state()==Item::sNone || (*it)->state()==Item::sProgress)
        {
            it2 = it;    
        }
        it++;
    }
    return ++it2;
}

Tree::iterator Tree::endState(Item::State state) const
{
    iterator it=beginState(state),it2=it;
    if (it==end())
    {
        return it;
    }
    it.setType(itNormal);
    while (it!=end())
    {
        if ((*it)->state()==state)
        {
            it2 = it;    
        }
        it++;
    }
    return ++it2;
}

int Tree::extractIndex(string &indices, bool forward)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos;
    if (forward)
    {
        pos = indices.find("_");
    }
    else
    {
        pos = indices.rfind("_");
    }
    if (pos==-1)    // this is the last (or first) index
    {
        buf << indices;
        indices = "";
    }
    else
    {
        if (forward)
        {
            buf << indices.substr(0,pos);
            indices = indices.substr(pos+1);
        }
        else
        {
            buf << indices.substr(pos+1);
            indices = indices.substr(0, pos);
        }
    }
    int n;
    buf >> n;
    return n;
}

Item* Tree::operator[](const string &indices)
{
    string sub(indices);
    int n = extractIndex(sub);
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw out_of_range("Index out of bounds");
    }
    if (sub=="")    // we are at the last tree
    {
        return vChildren[n]->item();
    }
    else
    {
        return vChildren[n]->tree()[sub];
    }
}

Branch* Tree::parent() const
{
    return pParent;
}

unsigned int Tree::numberOfChildren() const
{
    return vChildren.size();
}

Branch* Tree::branch(const string &indices)
{
    string sub(indices);
    int n = extractIndex(sub);
    if (n<0 || (unsigned int) n >= vChildren.size())
    {
        throw out_of_range("Index out of bounds");
    }
    if (sub=="")
    {
        return vChildren[n];
    }
    else
    {
        return vChildren[n]->tree().branch(sub);
    }
}

Branch* Tree::insert(int index, Item *item)
{
    if (index<0 || (unsigned int) index > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw out_of_range("Index out of bounds");
    }
    Branch *branch = new Branch(item,this);
    vChildren.insert(vChildren.begin()+index,branch);
    return branch;  // returns the branch just created
}

Branch* Tree::insert(const string &indices, Item *item)
{
    string sub(indices);
    int n = extractIndex(sub);
    if (sub=="")
    {
        return insert(n,item);
    }
    else
    {
        return vChildren[n]->tree().insert(sub,item);
    }
}

void Tree::insert(const string &indices, Branch *branch)
{ 
    string sub(indices);
    int n = extractIndex(sub);
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw out_of_range("Index out of bounds");
    }
    if (sub=="")
    {
        vChildren.insert(vChildren.begin()+n,branch);
        branch->setParent(this);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw out_of_range("Index out of bounds");
        }
        vChildren[n]->tree().insert(sub,branch);
    }
}

Branch* Tree::add(Item *item)
{
    Branch *branch = new Branch(item,this);
    vChildren.push_back(branch);
    return branch;
}

void Tree::remove(int index, bool toDelete)
{
    if (index<0 || (unsigned int) index >= vChildren.size())
    {
        throw out_of_range("Index out of bounds");
    }
    Branch *branch = vChildren[index];
    vChildren.erase(vChildren.begin()+index);
    if (toDelete)
    {
        delete branch;
    }
}

void Tree::remove(const string &indices, bool toDelete)
{
    string sub(indices);
    int n = extractIndex(sub);
    if (sub=="")
    {
        remove(n,toDelete);
    }
    else
    {
        vChildren[n]->tree().remove(sub,toDelete);
    }
}

bool Tree::move(const string &currentIndices, const string &newIndices)
{
    if (newIndices.find(currentIndices) == 0)
    {
        return false;
    }
    insert(newIndices,branch(currentIndices));
    // we now determine if there is need to modify the current indices for the suppression
    string subNew(newIndices), subCurrent(currentIndices);
    int nNew, nCurrent;
    ostringstream buf;
    // we iterate over the indices
    bool firstTime = true;
    do
    {
        nNew = extractIndex(subNew);
        nCurrent = extractIndex(subCurrent);
        
        if (!firstTime)
        {
            buf << "_";
        }

        if (subNew=="" && nNew <= nCurrent)
        {
            // we modify the indices of the item to remove
            buf << (nCurrent+1);
        }
        else
        {
            // we do not modify the indices of the item to remove
            buf << nCurrent;
        }
        firstTime = false;
    }
    while (subNew!="" && subCurrent!="" && nNew == nCurrent);

    // we add the following indices (they are unchanged)
    if (subCurrent!="")
    {
        buf << "_" << subCurrent;
    }
    remove(buf.str(),false);
    return true;
}

int Tree::indexOf(Branch *branch) const
{
    if (vChildren.size()==0)
    {
        return -1;  // not found
    }
    int res = 0;
    for (vector<Branch *>::const_iterator it = vChildren.begin(); it != vChildren.end() && *it != branch; it++)
    {
        res++;
    }
    if ((unsigned int)res < vChildren.size() && vChildren[res] == branch)
    {
        return res;
    }
    else
    {
        return -1;
    }
}

string Tree::indicesOf(Branch *branch) const
{
    ostringstream buf;
    int n = indexOf(branch);
    if (n == -1)
    {
        Tree *parent = branch->parent();
        if (parent && parent->pParent)
        {
            buf << indicesOf(parent->pParent) << "_" << parent->indexOf(branch);
        }
        else
        {
            throw out_of_range("No such a branch in the tree");
        }
    }
    else
    {
        buf << n;
    }
    return buf.str();
}

string Tree::indicesOfNext(Branch *branch) const
{
    ostringstream buf;
    int n = indexOf(branch);
    if (n == -1)
    {
        Tree *parent = branch->parent();
        if (parent && parent->pParent)
        {
            buf << indicesOf(parent->pParent) << "_" << (parent->indexOf(branch)+1);
        }
        else
        {
            return "0";
        }
    }
    else
    {
        buf << (n+1);
    }
    return buf.str();
}

// iterator's methods

Tree::iterator::iterator(const vector<vector<Branch*>::const_iterator>& its, IterationType type, Item::State state): itType(type), sState(state), qIts(its)
{
}

Tree::iterator::iterator(const vector<Branch*>::const_iterator& it, IterationType type, Item::State state): itType(type),sState(state)
{
	qIts.push_back(it);
    vIndices.push_back(0);
}

Tree::IterationType Tree::iterator::type() const
{
    return itType;
}

Item::State Tree::iterator::state() const
{
    return sState;
}

void Tree::iterator::setType(IterationType type)
{
    itType=type;
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
        throw out_of_range("End of tree reached");
    }
    if ((*(*it))->tree().vChildren.size()!=0)   // if there are children, go to the item() child
    {
        qIts.push_back((*(*it))->tree().vChildren.begin());
        vIndices.push_back(0);
    }
    else    // otherwise, go to the next (if there is one)
    {
        (*it)++;
        vIndices.back()++;
        while (it+1 != qIts.rend() && *it == (*(*(it+1)))->tree().vChildren.end())
        {
            qIts.pop_back();
            vIndices.pop_back();
            it++;
            (*it)++;
            vIndices.back()++;
        }
    }   
    return *this;
}

Tree::iterator Tree::iterator::operator++(int)
{
    iterator it = *this;
    operator++();
    return it;
}

Item* Tree::iterator::operator*()
{
    switch (type())
    {
        case itUnchecked:   while ((*(qIts.back()))->item()->state()==Item::sSuccess || (*(qIts.back()))->item()->state()==Item::sFailure)
                                operator++();
                            break; 
        case itState:
            {
                Item::State sState = state();
                while ((*(qIts.back()))->item()->state()!=sState)
                    operator++();
                break;
            }
        default:            break;
    }
    return (*(qIts.back()))->item();
}

int Tree::iterator::depth() const
{
    return qIts.size()-1;
}

Tree* Tree::iterator::parent() const
{
    try
    {
        return (*qIts.back())->parent();
    }
    catch(exception e)
    {
        return 0;
    }
}

Branch* Tree::iterator::branch() const
{
    try
    {
        return *qIts.back();
    }
    catch(exception e)
    {
        return 0;
    }
    
}

void Tree::setItem(string &indices, Item *item)
{
    branch(indices)->setItem(item);
}

string Tree::iterator::indices() const
{
    ostringstream buf;
    for (vector<int>::const_iterator it = vIndices.begin(); it != vIndices.end(); it++)
    {
        buf << *it;
        if (it+1 != vIndices.end())
        {
            buf << "_";
        }
    }
    return buf.str();
}
