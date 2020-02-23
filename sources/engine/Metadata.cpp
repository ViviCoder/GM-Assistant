/*************************************************************************
* Copyright © 2013-2020 Vincent Prat & Simon Nicolas
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
#include <Poco/DOM/Text.h>

using namespace std;

Metadata::Metadata()
{
}

void Metadata::fromXML(const Poco::XML::Element *root)
{
    Poco::XML::Element *element = root->getChildElement("title");
    if (element)
    {
        setTitle(element->getAttribute("value"));
    }
    element = root->getChildElement("author");
    if (element)
    {
        setAuthor(element->getAttribute("value"));
    }
    element = root->getChildElement("creation");
    if (element)
    {
        setCreationDate(element->getAttribute("date"));
    }
    element = root->getChildElement("description");
    if (element)
    {
        setDescription(element->innerText());
    }
    element = root->getChildElement("rpg");
    if (element)
    {
        setRpg(element->getAttribute("value"));
    }
    element = root->getChildElement("players");
    if (element)
    {
        setPlayers(element->getAttribute("value"));
    }
    element = root->getChildElement("game");
    if (element)
    {
        setGameDate(element->getAttribute("date"));
    }
}

void Metadata::toXML(Poco::XML::Element *root) const
{
    using namespace Poco::XML;

    Document *document = root->ownerDocument();
    Element *tmp = document->createElement("title");
    root->appendChild(tmp);
    tmp->setAttribute("value", sTitle);
    tmp = document->createElement("author");
    root->appendChild(tmp);
    tmp->setAttribute("value", sAuthor);
    tmp = document->createElement("creation");
    root->appendChild(tmp);
    stringstream bufCreation;
    bufCreation << dCreation.day() << "/" << dCreation.month() << "/" << dCreation.year();
    tmp->setAttribute("date", bufCreation.str());
    tmp = document->createElement("description");
    root->appendChild(tmp);
    tmp->appendChild(document->createTextNode(sDescription));
    tmp = document->createElement("rpg");
    root->appendChild(tmp);
    tmp->setAttribute("value", sRpg);
    tmp = document->createElement("players");
    root->appendChild(tmp);
    tmp->setAttribute("value", sPlayers);
    tmp = document->createElement("game");
    root->appendChild(tmp);
    stringstream bufGame;
    bufGame << dGame.day() << "/" << dGame.month() << "/" << dGame.year();
    tmp->setAttribute("date", bufGame.str());
}

bool Metadata::operator!=(const Metadata &metadata) const
{
    return (sTitle != metadata.sTitle || sAuthor != metadata.sAuthor || dCreation != metadata.dCreation || sDescription != metadata.sDescription || sRpg != metadata.sRpg || sPlayers != metadata.sPlayers || dGame != metadata.dGame);
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

Metadata::Date::Date(const string &date)
{
    istringstream buf(date);
    buf >> iDay;
    buf.ignore(1);
    buf >> iMonth;
    buf.ignore(1);
    buf >> iYear;
}

bool Metadata::Date::operator!=(const Date &date) const
{
    return (iDay != date.iDay || iMonth != date.iMonth || iYear != date.iYear);
}
