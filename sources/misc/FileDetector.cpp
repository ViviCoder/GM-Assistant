/*************************************************************************
* Copyright © 2013-2018 Vincent Prat & Simon Nicolas
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

#include "FileDetector.h"
#include <Poco/FileStream.h>

#define MAGIC_BUFFER_SIZE  4096

FileDetector::FileDetector(const std::string &installDir): mCookie(magic_open(MAGIC_MIME_TYPE)), pBuffer(new char[MAGIC_BUFFER_SIZE])
{
    magic_load(mCookie, 0);
    if (magic_error(mCookie))
    {
        magic_close(mCookie);
        mCookie = magic_open(MAGIC_MIME_TYPE);
        magic_load(mCookie, (installDir + "magic.mgc").c_str());
    }
}

FileDetector::~FileDetector()
{
    magic_close(mCookie);
    delete[] pBuffer;
}

std::string FileDetector::typeOfFile(const std::string &fileName) const
{
    std::string type = magic_file(mCookie, fileName.c_str());
    // On Windows, non-ASCII paths fail 
    if (type.substr(0, 11) == "cannot open")
    {
        Poco::FileInputStream stream(fileName);
        if (stream.good())
        {
            stream.get(pBuffer, MAGIC_BUFFER_SIZE);
            type = magic_buffer(mCookie, pBuffer, MAGIC_BUFFER_SIZE);
        }
    }
    return type;
}

bool FileDetector::isValid() const
{
    if (magic_error(mCookie))
    {
        return false;
    }
    return true;
}
