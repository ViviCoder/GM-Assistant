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

install_log="install.log"
cp_flags="-f"

echo "Installing GM-Assistant:"
echo

if [ -z $1 ]
then
    echo "No install directory given"
    exit
fi

# emptying the log file
> $install_log

# creating the target directory if it does not exist
if [ ! -d $1 ]
then
    mkdir -p $1
fi

# getting install directory's full path
current_dir=$(pwd)
cd $1
install_dir=$(pwd)
cd $current_dir
# storing it into a configuration file
echo $install_dir > ~/.GM-Assistant

# copying files
echo "Copying data..."
cp -rv $cp_flags data $1 >> $install_log
echo "Copying examples..."
cp -rv $cp_flags examples $1 >> $install_log
echo "Copying translations..."
if [ ! -d $1/translations ]
then
    mkdir -p $1/translations
fi
cp -v $cp_flags translations/*.qm $1/translations >> $install_log
echo "Copying executable..."
cp -v $cp_flags GM-Assistant $1 >> $install_log
echo "Copying license..."
cp -v $cp_flags LICENSE $1 >> $install_log
echo "Copying changelog..."
cp -v $cp_flags changelog $1 >> $install_log
echo
echo "Installation done"
echo "To uninstall, type 'make uninstall'"
