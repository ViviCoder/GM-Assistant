/*************************************************************************
* Copyright © 2011 Vincent Prat & Simon Nicolas
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

#include "FileItem.h"
#include <QFileInfo>

using namespace std;

FileItem::FileItem(const string &content, State state, const string &fileName): Item(content,state), sFileName(fileName)
{
    if (fileName != "")
    {
        setFileName(fileName);
    }
}

Item::Type FileItem::type() const
{
    return tFile;
}

string FileItem::fileName() const
{
    return sFileName;
}

void FileItem::setFileName(const string &fileName) throw(invalid_argument)
{
    sFileName = fileName;
    if (!QFileInfo(fileName.c_str()).exists())
    {
        throw invalid_argument("The file "+fileName+" does not exist.");
    }
}

void FileItem::fromXML(const xmlpp::Element &root) throw(xmlpp::exception, invalid_argument)
{
    using namespace xmlpp;
    
    Node::NodeList list = root.get_children("file");
    string name = "";
    if (list.size()==0)
    {
        throw xmlpp::exception("Missing file name");
    }
    else
    {
        Element *tmp = dynamic_cast<Element*>(list.front());
        Attribute *attr = tmp->get_attribute("name");
        if (attr!=NULL)
        {
            name = attr->get_value();
        }
    }
    setFileName(name);
}

void FileItem::toXML(xmlpp::Element &root)
{
    using namespace xmlpp;

    Element *tmp = root.add_child("file");
    tmp->set_attribute("name",sFileName);
}
