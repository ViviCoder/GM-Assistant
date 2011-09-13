#include "SoundItem.h"

using namespace std;

SoundItem::SoundItem(const string &content, State state): Item(content,state)
{
}

Item::Type SoundItem::type() const
{
    return tSound;
}

string SoundItem::fileName() const
{
    return sFileName;
}

void SoundItem::setFileName(const string &fileName)
{
    sFileName = fileName;
}

void SoundItem::fromXML(const xmlpp::Element &root) throw(xmlpp::exception)
{
    using namespace xmlpp;
    
    Node::NodeList list = root.get_children("file");
    if (list.size()==0)
    {
        throw xmlpp::exception("Missing file name");
    }
    else
    {
        Element *tmp = dynamic_cast<Element*>(list.front());
        sFileName = "";
        Attribute *attr = tmp->get_attribute("name");
        if (attr!=NULL)
        {
            sFileName = attr->get_value();
        }
    }
}

void SoundItem::toXML(xmlpp::Element &root)
{
    using namespace xmlpp;

    Element *tmp = root.add_child("file");
    tmp->set_attribute("name",sFileName);
}
