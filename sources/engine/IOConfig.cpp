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

#include "IOConfig.h"
#include <libxml++/libxml++.h>

using namespace std;
using namespace xmlpp;

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
    }
    else
    {
        sRootName = "scenario";
        sPlotName = "plot";
        sPropertiesName = "properties";
        sImageName = "image";
        sPropertyName = "property";
        bHasExpanded = true;
    }
}

string IOConfig::rootName() const
{
    return sRootName;
}

string IOConfig::plotName() const
{
    return sPlotName;
}

string IOConfig::propertiesName() const
{
    return sPropertiesName;
}

string IOConfig::imageName() const
{
    return sImageName;
}

Version IOConfig::version() const
{
    return vVersion;
}

IOConfig IOConfig::detect(const string &fileName)
{
    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    Attribute *attr = root->get_attribute("version");
    Version version;
    if (attr)
    {
        version = Version(attr->get_value());
    }
    IOConfig res(version);
    string newRoot = root->get_name();
    if (newRoot == "game" || newRoot == "scenario")
    {
        res.setRootName(newRoot);
    }
    if (!root->get_children("plot").empty())
    {
        res.setPlotName("plot");
    }
    else if (!root->get_children("scenario").empty())
    {
        res.setPlotName("scenario");
    }
    if (!root->get_children("properties").empty())
    {
        res.setPropertiesName("properties");
        res.setPropertyName("property");
    }
    else if (!root->get_children("skills").empty())
    {
        res.setPropertiesName("skills");
        res.setPropertyName("skill");
    }
    if (!root->find("//item[@type='image']").empty())
    {
        res.setImageName("image");
        res.setHasImages(true);
    }
    else if (!root->find("//item[@type='picture']").empty())
    {
        res.setImageName("picture");
        res.setHasImages(true);
    }
    if (!root->find("//item[@expanded]").empty())
    {
        res.setHasExpanded(true);
    }
    return res;
}

string IOConfig::propertyName() const
{
    return sPropertyName;
}

bool IOConfig::hasImages() const
{
    return bHasImages;
}

bool IOConfig::hasExpanded() const
{
    return bHasExpanded;
}

bool IOConfig::isValid() const
{
    return bValid;
}

void IOConfig::setRootName(const string &rootName)
{
    if (rootName != sRootName)
    {
        sRootName = rootName;
        bValid = false;
    }
}

void IOConfig::setPlotName(const string &plotName)
{
    if (plotName != sPlotName)
    {
        sPlotName = plotName;
        bValid = false;
    }
}

void IOConfig::setPropertiesName(const string &propertiesName)
{
    if (propertiesName != sPropertiesName)
    {
        sPropertiesName = propertiesName;
        bValid = false;
    }
}

void IOConfig::setPropertyName(const string &propertyName)
{
    if (propertyName != sPropertyName)
    {
        sPropertyName = propertyName;
        bValid = false;
    }
}

void IOConfig::setImageName(const string &imageName)
{
    if (imageName != sImageName)
    {
        sImageName = imageName;
        bValid = false;
    }
}

void IOConfig::setHasImages(bool hasImages)
{
    if (hasImages != bHasImages)
    {
        bHasImages = hasImages;
        bValid = false;
    }
}

void IOConfig::setHasExpanded(bool hasExpanded)
{
    if (hasExpanded != bHasExpanded)
    {
        bHasExpanded = hasExpanded;
        bValid = false;
    }
}
