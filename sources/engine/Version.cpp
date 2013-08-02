/*************************************************************************
* Copyright © 2012-2013 Vincent Prat & Simon Nicolas
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

#include "Version.h"
#include <sstream>

using namespace std;

Version::Version(int major, int minor, int release): iMajor(major), iMinor(minor), iRelease(release)
{
}

Version::Version(const string &version): iMajor(MAJOR_VERSION), iMinor(MINOR_VERSION), iRelease(RELEASE_VERSION)
{
    istringstream buf(version);
    buf >> iMajor;
    buf.ignore(1);
    buf >> iMinor;
    buf.ignore(1);
    buf >> iRelease;
}

string Version::shortVersion()
{
    ostringstream buf;
    buf << iMajor << "." << iMinor;
    return buf.str();
}

string Version::fullVersion()
{
    ostringstream buf;
    buf << iMajor << "." << iMinor << "." << iRelease;
    return buf.str();
}

bool Version::operator<(const Version &version)
{
    if (iMajor < version.iMajor)
    {
        return true;
    }
    if (iMajor > version.iMajor)
    {
        return false;
    }
    if (iMinor < version.iMinor)
    {
        return true;
    }
    return false;
}

bool Version::operator>(const Version &version)
{
    if (iMajor > version.iMajor)
    {
        return true;
    }
    if (iMajor < version.iMajor)
    {
        return false;
    }
    if (iMinor > version.iMinor)
    {
        return true;
    }
    return false;
}
