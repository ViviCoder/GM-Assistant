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

#include "IOConfig.h"
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/Path.h>

using namespace std;
using namespace Poco::XML;

IOConfig::IOConfig(const Version &version): vVersion(version), bValid(true)
{
    if (vVersion < Version(1, 1))
    {
        bHasImages = false;
    }
    else
    {
        bHasImages = true;
    }
    if (vVersion < Version(1, 2))
    {
        sRootName = "game";
        sPlotName = "scenario";
        sPropertiesName = "skills";
        sImageName = "picture";
        sPropertyName = "skill";
        bHasExpanded = false;
        bHasMetadata = false;
        bArchived = false;
        sDescriptionName = "playername";
    }
    else
    {
        sRootName = "scenario";
        sPlotName = "plot";
        sPropertiesName = "properties";
        sImageName = "image";
        sPropertyName = "property";
        bHasExpanded = true;
        bHasMetadata = true;
        bArchived = true;
        sDescriptionName = "description";
    }
}

IOConfig IOConfig::detect(const string &fileName, bool isArchived)
{
    DOMParser parser;
    Document *document = parser.parse(fileName);
    Element *root = document->documentElement();
    string attr = root->getAttribute("version");
    Version version;
    if (!attr.empty())
    {
        version = Version(attr);
    }
    IOConfig res(version);
    res.setArchived(isArchived);
    // Temporary directory
    if (isArchived)
    {
        res.sTempDir = Poco::Path(fileName).parent().toString();
    }
    string newRoot = root->localName();
    // game or scenario
    if (newRoot == "game" || newRoot == "scenario")
    {
        res.setRootName(newRoot);
    }
    // metadata
    if (root->getChildElement("metadata"))
    {
        res.setHasMetadata(true);
    }
    // plot or scenario
    if (root->getChildElement("plot"))
    {
        res.setPlotName("plot");
    }
    else if (root->getChildElement("scenario"))
    {
        res.setPlotName("scenario");
    }
    // propertie or skill
    if (root->getChildElement("properties"))
    {
        res.setPropertiesName("properties");
        res.setPropertyName("property");
    }
    else if (root->getChildElement("skills"))
    {
        res.setPropertiesName("skills");
        res.setPropertyName("skill");
    }
    // image or picture
    if (root->getNodeByPath("//item[@type='image']"))
    {
        res.setImageName("image");
        res.setHasImages(true);
    }
    else if (root->getNodeByPath("//item[@type='picture']"))
    {
        res.setImageName("picture");
        res.setHasImages(true);
    }
    // expanded
    if (root->getNodeByPath("//item[@expanded]"))
    {
        res.setHasExpanded(true);
    }
    // description or playername
    if (root->getNodeByPath("//character[@description]"))
    {
        res.setDescriptionName("description");
    }
    else if (root->getNodeByPath("//character[@playername]"))
    {
        res.setDescriptionName("playername");
    }
    return res;
}
