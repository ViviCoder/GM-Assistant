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

#include "FileItem.h"
#include <QFileInfo>

using namespace std;

FileItem::FileItem(const string &content, State state, const string &fileName, bool sizeLimited, int limitSize): Item(content,state), sFileName(fileName), bSizeLimited(sizeLimited), iLimitSize(limitSize)
{
    if (fileName != "")
    {
        setFileName(fileName,true);
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

void FileItem::setFileName(const string &fileName, bool checkFile) throw(invalid_argument, overflow_error)
{
    sFileName = fileName;
    QFileInfo file(fileName.c_str());
    if (checkFile && !file.exists())
    {
        throw invalid_argument("The file "+fileName+" does not exist.");
    }
    if (bSizeLimited && file.size()/1024 > iLimitSize)
    {
        throw overflow_error((QString("The file ") + fileName.c_str() + " exceeds the size limit of %1MB.").arg(iLimitSize/1024).toStdString());
    }
}

void FileItem::fromXML(const xmlpp::Element &root, bool checkFile) throw(xmlpp::exception, invalid_argument, overflow_error)
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
    setFileName(name, checkFile);
}

void FileItem::toXML(xmlpp::Element &root)
{
    using namespace xmlpp;

    Element *tmp = root.add_child("file");
    tmp->set_attribute("name",sFileName);
}
