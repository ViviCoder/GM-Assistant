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

#ifndef HEADER_VERSION
#define HEADER_VERSION

#define MAJOR_VERSION   1
#define MINOR_VERSION   0
#define RELEASE_VERSION 13

// some stringification macros
#define xstr(s) str(s)
#define str(s)  #s

// version string
#define SHORT_VERSION   xstr(MAJOR_VERSION)"."xstr(MINOR_VERSION)
#define FULL_VERSION    SHORT_VERSION"."xstr(RELEASE_VERSION)

#endif
