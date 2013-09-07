/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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

#include "Metadata.h"
#include <ctime>
#include <sstream>

using namespace std;

Metadata::Metadata()
{
}

string Metadata::title() const
{
    return sTitle;
}

void Metadata::setTitle(const string &title)
{
    sTitle = title;
}

string Metadata::author() const
{
    return sAuthor;
}

void Metadata::setAuthor(const string &author)
{
    sAuthor = author;
}

Metadata::Date Metadata::creationDate() const
{
    return dCreation;
}

void Metadata::setCreationDate(const Date &creationDate)
{
    dCreation = creationDate;
}

string Metadata::description() const
{
    return sDescription;
}

void Metadata::setDescription(const string &description)
{
    sDescription = description;
}

string Metadata::players() const
{
    return sPlayers;
}

void Metadata::setPlayers(const string &players)
{
    sPlayers = players;
}

Metadata::Date Metadata::gameDate() const
{
    return dGame;
}

void Metadata::setGameDate(const Date &gameDate)
{
    dGame = gameDate;
}

void Metadata::fromXML(const xmlpp::Element &root)
{
    using namespace xmlpp;

    Attribute *attr;
    Node::NodeList node = root.get_children("title");
    if (!node.empty())
    {
        attr = dynamic_cast<Element*>(node.front())->get_attribute("value");
        if (attr)
        {
            setTitle(attr->get_value());
        }
    }
    node = root.get_children("author");
    if (!node.empty())
    {
        attr = dynamic_cast<Element*>(node.front())->get_attribute("value");
        if (attr)
        {
            setAuthor(attr->get_value());
        }
    }
    node = root.get_children("creation");
    if (!node.empty())
    {
        attr = dynamic_cast<Element*>(node.front())->get_attribute("date");
        if (attr)
        {
            setCreationDate(Date(attr->get_value()));
        }
    }
    node = root.get_children("description");
    if (!node.empty())
    {
        Element *tmp = dynamic_cast<Element*>(node.front());
        if (tmp->has_child_text())
        {
            setDescription(tmp->get_child_text()->get_content());
        }
    }
    node = root.get_children("players");
    if (!node.empty())
    {
        attr = dynamic_cast<Element*>(node.front())->get_attribute("value");
        if (attr)
        {
            setPlayers(attr->get_value());
        }
    }
    node = root.get_children("game");
    if (!node.empty())
    {
        attr = dynamic_cast<Element*>(node.front())->get_attribute("date");
        if (attr)
        {
            setGameDate(Date(attr->get_value()));
        }
    }
}

void Metadata::toXML(xmlpp::Element &root) const
{
    using namespace xmlpp;

    Element *tmp = root.add_child("title");
    tmp->set_attribute("value", sTitle);
    tmp = root.add_child("author");
    tmp->set_attribute("value", sAuthor);
    tmp = root.add_child("creation");
    stringstream bufCreation;
    bufCreation << dCreation.day() << "/" << dCreation.month() << "/" << dCreation.year();
    tmp->set_attribute("date", bufCreation.str());
    tmp = root.add_child("description");
    tmp->add_child_text(sDescription);
    tmp = root.add_child("players");
    tmp->set_attribute("value", sPlayers);
    tmp = root.add_child("game");
    stringstream bufGame;
    bufGame << dGame.day() << "/" << dGame.month() << "/" << dGame.year();
    tmp->set_attribute("date", bufGame.str());
}

// Date methods

Metadata::Date::Date()
{
    time_t currentTime;
    time(&currentTime);
    struct tm *time_info = localtime(&currentTime);
    iDay = time_info->tm_mday;
    iMonth = time_info->tm_mon + 1;
    iYear = time_info->tm_year + 1900;
}

Metadata::Date::Date(int day, int month, int year): iDay(day), iMonth(month), iYear(year)
{
}

int Metadata::Date::day() const
{
    return iDay;
}

int Metadata::Date::month() const
{
    return iMonth;
}

int Metadata::Date::year() const
{
    return iYear;
}

Metadata::Date::Date(const string &date)
{
    istringstream buf(date);
    buf >> iDay;
    buf.ignore(1);
    buf >> iMonth;
    buf.ignore(1);
    buf >> iYear;
}