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

#include "FileDetector.h"

FileDetector::FileDetector(const std::string &installDir): mCookie(magic_open(MAGIC_MIME_TYPE))
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
}

std::string FileDetector::typeOfFile(const std::string &fileName) const
{
    return magic_file(mCookie, fileName.c_str());
}

bool FileDetector::isValid() const
{
    if (magic_error(mCookie))
    {
        return false;
    }
    return true;
}
