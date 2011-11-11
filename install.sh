##########################################################################
# Copyright © 2011 Vincent Prat & Simon Nicolas
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
##########################################################################

#!/bin/sh

echo "Installing GM-Assistant:"
echo

if [ -z $1 ]
then
    echo "No install directory given"
    exit
fi

if [ ! -d $1 ]
then
    mkdir -p $1
fi

echo "Copying data..."
cp -r data $1
echo "Copying examples..."
cp -r examples $1
echo "Copying translations..."
if [ ! -d $1/translations ]
then
    mkdir $1/translations
fi
cp translations/*.qm $1/translations
echo "Copying executable..."
cp GM-Assistant $1
echo "Copying license..."
cp LICENSE $1
echo "Copying changelog..."
cp changelog $1
echo
echo "Installation done"
