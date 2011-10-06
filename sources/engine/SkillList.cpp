#include "SkillList.h"

using namespace std;

SkillList::SkillList()
{
}

void SkillList::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    for (vector<string>::const_iterator it = vSkills.begin(); it != vSkills.end(); it++)
    {
        Element *tmp = root.add_child("skill");
        tmp->set_attribute("name",*it);
    }
}

void SkillList::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    clear();
    Node::NodeList node = root.get_children("skill");
    for (Node::NodeList::const_iterator it = node.begin(); it != node.end(); it++)
    {
        Element *elem = dynamic_cast<Element*>(*it);
        string name;
        Attribute *attr = elem->get_attribute("name");
        if (attr != NULL)
        {
            name = attr->get_value();
        }
        vSkills.push_back(name);
    }
}

void SkillList::clear()
{
    vSkills.clear();
}

void SkillList::add(const std::string &skill)
{
    vSkills.push_back(skill);
}

void SkillList::remove(int index) throw(out_of_range)
{
    if (index < 0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    vSkills.erase(vSkills.begin()+index);
}

std::string& SkillList::operator[](int index) throw(out_of_range)
{
    if (index<0 || (unsigned int)index >= vSkills.size())
    {
        throw out_of_range("Index out of bounds");
    }
    return vSkills[index];
}
