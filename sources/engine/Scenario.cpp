/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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

#include "Scenario.h"
#include "Version.h"

using namespace std;

// constructors

Scenario::Scenario()
{
}

Scenario::Scenario(const string &fileName) throw(xmlpp::exception)
{
    try
    {
        fromFile(fileName);
    }
    catch (xmlpp::exception)
    {
        clear();
        throw;
    }
}

// XML methods

void Scenario::fromFile(const std::string &fileName) throw(xmlpp::exception, invalid_argument, overflow_error)
{
    using namespace xmlpp;

    clear();
    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name()!="game")
    {
        throw xmlpp::exception("Bad document content type: game expected");
    }
    // getting the user interface
    try
    {
        Attribute *attr = root->get_attribute("interface");
        if (attr != NULL)
        {
            uiInterface = stringToInterface(attr->get_value());
        }
        else
        {
            uiInterface = uiFull;
        }
    }
    catch (invalid_argument)
    {
        throw xmlpp::exception("Bad user interface");
    }
    // now loading the different parts of the game
    Node::NodeList node = root->get_children("scenario");
    if (!node.empty())
    {
        tScenario.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("notes");
    if (!node.empty())
    {
        Element *elem = dynamic_cast<Element*>(node.front());
        if (elem->has_child_text())
        {
            sNotes = elem->get_child_text()->get_content();
        }
        else
        {
            sNotes = "";
        }
    }
    node = root->get_children("skills");
    if (!node.empty())
    {
        lSkills.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("characters");
    if (!node.empty())
    {
        lCharacters.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("history");
    if (!node.empty())
    {
        tHistory.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("music");
    if (!node.empty())
    {
        tMusic.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("effects");
    if (!node.empty())
    {
        tEffects.fromXML(*dynamic_cast<Element*>(node.front()),true);
    }
}

void Scenario::toFile(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node("game");
    root->set_attribute("version",Version::currentVersion().shortVersion());
    root->set_attribute("interface",interfaceToString(uiInterface));
    Element *tmp = root->add_child("scenario");
    tScenario.toXML(*tmp);
    tmp = root->add_child("notes");
    tmp->add_child_text(sNotes);
    tmp = root->add_child("skills");
    lSkills.toXML(*tmp);
    tmp = root->add_child("characters");
    lCharacters.toXML(*tmp);
    tmp = root->add_child("history");
    tHistory.toXML(*tmp);
    tmp = root->add_child("music");
    tMusic.toXML(*tmp);
    tmp = root->add_child("effects");
    tEffects.toXML(*tmp);
    document.write_to_file_formatted(fileName,"UTF-8");
}

// accessors

Tree& Scenario::scenario()
{
    return tScenario;
}

string& Scenario::notes()
{
    return sNotes;
}

Tree& Scenario::history()
{
    return tHistory;
}

Tree& Scenario::music()
{
    return tMusic;
}

Tree& Scenario::effects()
{
    return tEffects;
}

// methods

void Scenario::clear()
{
    tScenario.clear();
    sNotes = "";
    tHistory.clear();
    tMusic.clear();
    tEffects.clear();
    lSkills.clear();
    lCharacters.clear();
}

CharacterList& Scenario::characters()
{
    return lCharacters;
}

SkillList& Scenario::skills()
{
    return lSkills;
}

string Scenario::interfaceToString(UserInterface interface)
{
    switch (interface)
    {
        case uiFull:    return "full";
        case uiSimple:  return "simple";
        case uiMusic:   return "music";
        case uiDesign:  return "design";
        default:        return "nomusic";
    }
}

Scenario::UserInterface Scenario::stringToInterface(const std::string& interface) throw(invalid_argument)
{
    if (interface=="full")
        return uiFull;
    else if (interface=="simple")
        return uiSimple;
    else if (interface=="music")
        return uiMusic;
    else if (interface=="design")
        return uiDesign;
    else if (interface=="nomusic")
        return uiNoMusic;
    else
        throw invalid_argument("Invalid user interface");
}

Scenario::UserInterface Scenario::userInterface() const
{
    return uiInterface;
}

void Scenario::setUserInterface(Scenario::UserInterface interface)
{
    uiInterface = interface;
}
