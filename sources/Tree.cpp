#include "Tree.h"
#include <sstream>
// for debugging
//#include <iostream>


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

Tree::Tree(const xmlpp::Element &root, Branch* parent): pParent(parent)
{
    fromXML(root);
}

Tree::Tree(const string &fileName) throw(xmlpp::exception): pParent(NULL)
{
    try
    {
        fromXML(fileName);
    }
    catch (xmlpp::exception)
    {
        clear();
        throw;
    }
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
        Branch *branch = new Branch(**it,pParent);
        vChildren.push_back(branch);
    }
    pParent = NULL;
    return *this;
}

// inherited methods

void Tree::toXML(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node("tree");
    toXML(*root);
    document.write_to_file_formatted(fileName,"UTF-8");
}

void Tree::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<Branch*>::const_iterator it=vChildren.begin(); it != vChildren.end(); it++)
    {
        Element *tmp = root.add_child("item");
        stringstream buf(stringstream::in | stringstream::out);
        buf << (*it)->item()->state();
        tmp->set_attribute("state",buf.str());
        tmp->set_child_text((*it)->item()->content());
        (*it)->tree().toXML(*tmp);
    }
}

void Tree::fromXML(const string &fileName) throw(xmlpp::exception)
{
    using namespace xmlpp;
    
    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name()!="tree")
    {
        throw xmlpp::exception("Bad document content type: tree expected");
    }
    fromXML(*root);
}

void Tree::fromXML(const xmlpp::Element &root)
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
        Branch *branch = new Branch(new Item(elem->get_child_text()->get_content(),State(state)),*elem,pParent);
        vChildren.push_back(branch);
    }
}

void Tree::clear()
{
    for (vector<Branch*>::const_iterator it=vChildren.begin(); it!=vChildren.end(); it++)
    {
        delete *it;
    }
    vChildren.clear();
}

// new methods

Tree::iterator Tree::begin() const
{
    return iterator(vChildren.begin());
}

Tree::iterator Tree::beginUnchecked() const
{
    iterator it(vChildren.begin());
    while (it != end() && ((*it)->state()==sSuccess || (*it)->state()==sFailure))
    {
        it++;
    }
    it.setType(itUnchecked);
    return it;
}

Tree::iterator Tree::beginState(State state) const
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
        if ((*it)->state()==sNone || (*it)->state()==sProgress)
        {
            it2 = it;    
        }
        it++;
    }
    return ++it2;
}

Tree::iterator Tree::endState(State state) const
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

int Tree::extractIndex(string &indices)
{
    stringstream buf(stringstream::in | stringstream::out);
    int pos = indices.find("_");
    if (pos==-1)    // this is the last index
    {
        buf << indices;
        indices = "";
    }
    else
    {
        buf << indices.substr(0,pos);
        indices = indices.substr(pos+1);
    }
    int n;
    buf >> n;
    return n;
}

Item* Tree::operator[](const string &indices) throw(out_of_range)
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

Branch* Tree::branch(const string &indices) throw(out_of_range)
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

void Tree::insert(const string &indices, const string &content, State state) throw(out_of_range)
{
    string sub(indices);
    int n = extractIndex(sub);
    if (n<0 || (unsigned int) n > vChildren.size()) // n can be equal to vChildren.size() but only for the last index
    {
        throw out_of_range("Index out of bounds");
    }
    if (sub=="")
    {
        Branch *branch = new Branch(new Item(content,state),pParent);
        vChildren.insert(vChildren.begin()+n,branch);
    }
    else
    {
        if ((unsigned int)n==vChildren.size())
        {
            throw out_of_range("Index out of bounds");
        }
        vChildren[n]->tree().insert(sub,content,state);
    }
}

void Tree::insert(const string &indices, Branch *branch) throw(out_of_range)
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
        branch->tree().pParent = pParent;
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

void Tree::add(const string &content, State state)
{
    if (vChildren.size()==0 || vChildren.back()->tree().vChildren.size()==0)
    {
        Branch *branch = new Branch(new Item(content,state),pParent);
        vChildren.push_back(branch);
    }
    else
    {
        vChildren.back()->tree().add(content,state);
    }
}

void Tree::add(int depth, const string &content, State state) throw(out_of_range)
{
    // insert at the given depth (1 is root)
    if (depth<0)
    {
        throw out_of_range("Depth out of range");
    }
    if (depth==0)
    {
        Branch *branch = new Branch(new Item(content,state),pParent);
        vChildren.push_back(branch);
    }
    else
    {
        if (vChildren.size()==0)
        {
            throw out_of_range("Depth out of range");
        }
        vChildren.back()->tree().add(depth-1,content,state);
    }
}

void Tree::addChild(const string &content, State state)
{
    if (vChildren.size()==0)
    {
        Branch *branch = new Branch(new Item(content,state),pParent);
        vChildren.push_back(branch);
    }
    else
    {
        vChildren.back()->tree().addChild(content,state);
    }
}

void Tree::remove(int index, bool toDelete) throw(out_of_range)
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

void Tree::move(const string &currentIndices, const string &newIndices)
{
    stringstream buf(stringstream::in | stringstream::out);
    insert(newIndices,branch(currentIndices));
    // we now determine if there is need to modify newIndices for the suppression
    int pos = currentIndices.rfind("_");
    int pos2 = newIndices.rfind("_");
    string indices(currentIndices);
    if ((pos==-1 && pos2==-1) || currentIndices.substr(0,pos)==newIndices.substr(0,pos2))
    {   // if both indices are "brothers"
        int n,n2;
        buf << currentIndices.substr(pos+1) << " " << newIndices.substr(pos2+1);
        buf >> n >> n2;
        if (n>n2)
        {
            stringstream buf2(stringstream::in | stringstream::out);
            buf2 << currentIndices.substr(0,pos+1) << (n+1);
            indices = buf2.str();
        }
    }
    remove(indices,false);    
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

// iterator's methods

Tree::iterator::iterator(const vector<vector<Branch*>::const_iterator>& its, IterationType type, State state): itType(type), sState(state), qIts(its)
{
}

Tree::iterator::iterator(const vector<Branch*>::const_iterator& it, IterationType type, State state): itType(type),sState(state)
{
	qIts.push_back(it);
}

Tree::IterationType Tree::iterator::type() const
{
    return itType;
}

State Tree::iterator::state() const
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

Tree::iterator& Tree::iterator::operator++() throw(out_of_range)
{
    vector<vector<Branch*>::const_iterator>::reverse_iterator it = qIts.rbegin();
    if (it==qIts.rend())
    {
        throw out_of_range("End of tree reached");
    }
    if ((*(*it))->tree().vChildren.size()!=0)   // if there are children, go to the item() child
    {
        qIts.push_back((*(*it))->tree().vChildren.begin());
    }
    else    // otherwise, go to the next (if there is one)
    {
        (*it)++;
        while (it+1 != qIts.rend() && *it == (*(*(it+1)))->tree().vChildren.end())
        {
            qIts.pop_back();
            it++;
            (*it)++;
        }
    }   
    return *this;
}

Tree::iterator Tree::iterator::operator++(int i)
{
    iterator it = *this;
    operator++();
    return it;
}

Item* Tree::iterator::operator*()
{
    switch (type())
    {
        case itUnchecked:   while ((*(qIts.back()))->item()->state()==sSuccess || (*(qIts.back()))->item()->state()==sFailure)
                                operator++();
                            break; 
        case itState:
            {
                State sState = state();
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

Branch* Tree::iterator::parent() const
{
    try
    {
        return (*qIts.back())->parent();
    }
    catch(exception e)
    {
        return NULL;
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
        return NULL;
    }
    
}
