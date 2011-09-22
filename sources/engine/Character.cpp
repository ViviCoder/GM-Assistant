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

std::string& Character::skill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vSkills[index];
}

// methods

void Character::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<std::string>::const_iterator it = vSkills.begin(); it != vSkills.end(); it++)
    {
        Element *tmp = root.add_child("skill");
        tmp->set_attribute("value",*it);
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
        string value;
        Attribute *attr = elem->get_attribute("value");
        if (attr != NULL)
        {
            value = attr->get_value();
        }
        vSkills.push_back(value);
    }
}

void Character::addSkill(const std::string &skill)
{
    vSkills.push_back(skill);
}

void Character::removeSkill(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vSkills.erase(vSkills.begin()+index);
}

void Character::clearSkills()
{
    vSkills.clear();
}
