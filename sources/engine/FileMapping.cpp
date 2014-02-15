/*************************************************************************
* Copyright © 2013-2014 Vincent Prat & Simon Nicolas
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
#include <Poco/FileStream.h>
#include <Poco/DigestStream.h>
#include <Poco/StreamCopier.h>

using namespace std;
using namespace Poco;

string FileMapping::addFile(const string &fileName, const string &destination)
{
    map<string, string>::const_iterator pos = mMapping.find(fileName);
    if (pos != mMapping.end())
    {
        return pos->second;
    }
    // test if the file has already been added (even with a different file name)
    string hash("x");
    try
    {
        FileInputStream ibuf(fileName.c_str());
        DigestOutputStream digestStream(md5Engine);
        StreamCopier::copyStream(ibuf, digestStream);
        digestStream.close();
        // computing MD5 hash to compare files
        hash = DigestEngine::digestToHex(md5Engine.digest());
        pos = mHashes.find(hash);
        if (pos != mHashes.end())
        {
            return pos->second;
        }
    }
    catch(...){}
    // otherwise, add it (but beware of existing name)
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
    mHashes.insert(pair<string, string>(hash, result));
    sAddedFiles.insert(result);
    return result;
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
