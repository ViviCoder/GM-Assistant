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

#include "FileMapping.h"
#include <Poco/Path.h>
#include <sstream>

using namespace std;
using namespace Poco;

string FileMapping::addFile(const string &fileName, const string &destination)
{
    map<string, string>::const_iterator pos = mMapping.find(fileName);
    if (pos == mMapping.end())
    {
        Path target(destination);
        string baseName = target.getBaseName();
        int i = 2;
        while (sAddedFiles.find(target.toString()) != sAddedFiles.end())
        {
            stringstream buf;
            buf << i;
            target.setBaseName(baseName + "_" + buf.str()); 
            i++;
        }
        string result(target.toString());
        mMapping.insert(pair<string, string>(fileName, result));
        sAddedFiles.insert(result);
        return result;
    }
    else
    {
        return pos->second;
    }
}

// iterator methods

FileMapping::Iterator::Iterator(const map<string, string>::const_iterator &it): itMap(it)
{
}

FileMapping::Iterator FileMapping::Iterator::operator++()
{
    itMap++;
    return *this;
}

FileMapping::Iterator FileMapping::Iterator::operator++(int)
{
    Iterator it(*this);
    itMap++;
    return it;
}
