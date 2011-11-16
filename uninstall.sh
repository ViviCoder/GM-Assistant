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

echo "Uninstalling GM-Assistant..."

dirs="" # directories to be deleted
log=$(cat $install_log | tr " " "@")

# handling install log
for line in $log
do
    file=$(echo $line | sed "s#.*@->@##" | sed "s/« //" | sed "s/ »//")
    if [ -d $file ]
    then
        dirs="$dirs $file"
    else
        rm -v $file
        # directory
        dirs=$dirs" $(echo $file | sed 's#\/[^\/]\{1,\}$##')"
    fi
done

# empty directories
dirs=$(echo $dirs | tr " " "\n" | sort -ur)

for dir in $dirs
do
    rmdir -v $dir
done

echo "Done"
