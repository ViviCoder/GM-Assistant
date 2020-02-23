/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/XML/XMLException.h>
#include <Poco/DOM/Document.h>

using namespace std;

FileItem::FileItem(const string &content, State state, bool expanded, const string &fileName): Item(content,state,expanded), sFileName(fileName), bIncluded(false)
{
    if (fileName != "")
    {
        setFileName(fileName,true);
    }
}

void FileItem::setFileName(const string &fileName, bool checkFile)
{
    sFileName = fileName;
    Poco::File file(fileName.c_str());
    if (checkFile && !file.exists())
    {
        throw invalid_argument("The file "+fileName+" does not exist.");
    }
}

void FileItem::fromXML(const IOConfig &config, const Poco::XML::Element *root, bool checkFile)
{
    using namespace Poco::XML;
    
    Element *elem = root->getChildElement("file");
    string name;
    if (elem)
    {
        name = elem->getAttribute("name");
    }
    else
    {
        throw Poco::XML::XMLException("Missing file name");
    }
    if (config.isArchived())
    {
        bIncluded = true;
        name = config.temporaryDirectory() + subdirectory() + name;
    }
    setFileName(name, checkFile);
}

void FileItem::toXML(const IOConfig &config, Poco::XML::Element *root, FileMapping &fileMapping)
{
    using namespace Poco::XML;

    Document *document = root->ownerDocument();
    Element *tmp = document->createElement("file");
    root->appendChild(tmp);
    std::string fileName(sFileName);
    if (config.isArchived())
    {
        fileName = Poco::Path(sFileName).getFileName();
        fileName = Poco::Path(fileMapping.addFile(sFileName, subdirectory() + fileName)).getFileName();
    }
    tmp->setAttribute("name", fileName);
}
