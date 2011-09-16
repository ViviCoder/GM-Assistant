#include "Character.h"

using namespace std;

// constructors

Character::Character(const string &name): sName(name)
{
}

Character::Character(const string &name, const string &playerName): sName(name), sPlayerName(playerName)
{
}

// accessors

string Character::name() const
{
    return sName;
}

string Character::playerName() const
{
    return sPlayerName;
}

bool Character::isPlaying() const
{
    return sPlayerName == "";
}

Skill& Character::skill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkill.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vSkill[index];
}

// methods

void Character::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<Skill>::const_iterator it = vSkill.begin(); it != vSkill.end(); it++)
    {
        Element *tmp = root.add_child("skill");
        tmp->set_attribute("name",it->name());
        stringstream buf(stringstream::in | stringstream::out);
        buf << it->value();
        tmp->set_attribute("value",buf.str());
    }
}

void Character::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    clearSkills();
    Node::NodeList list = root.get_children("skill");
    for (Node::NodeList::const_iterator it = list.begin(); it != list.end(); it++)
    {
        Element *elem = dynamic_cast<Element *>(*it);
        string name="";
        Attribute *attr = elem->get_attribute("name");
        if (attr != NULL)
        {
            name = attr->get_value();
        }
        int value = 0;
        attr = elem->get_attribute("value");
        if (attr != NULL)
        {
            stringstream buf(stringstream::in | stringstream::out);
            buf << attr->get_value();
            buf >> value;
        }
        vSkill.push_back(Skill(name,value));
    }
}

void Character::addSkill(const Skill &skill)
{
    vSkill.push_back(skill);
}

void Character::removeSkill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkill.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vSkill.erase(vSkill.begin()+index);
}

void Character::clearSkills()
{
    vSkill.clear();
}
