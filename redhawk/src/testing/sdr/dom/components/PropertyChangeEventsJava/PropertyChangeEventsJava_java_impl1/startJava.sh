#!/bin/sh
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file 
# distributed with this source distribution.
# 
# This file is part of REDHAWK core.
# 
# REDHAWK core is free software: you can redistribute it and/or modify it under 
# the terms of the GNU Lesser General Public License as published by the Free 
# Software Foundation, either version 3 of the License, or (at your option) any 
# later version.
# 
# REDHAWK core is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
# 
# You should have received a copy of the GNU Lesser General Public License 
# along with this program.  If not, see http://www.gnu.org/licenses/.
#

myDir=`dirname $0`
classpath=$myDir/EventReceive_java_impl1.jar:$myDir/PropertyChangeEventsJava_java_impl1:$myDir/bin
libdir=${SDRROOT}/../../base/framework/java
for jar in $(readlink -e $libdir/ossie/*.jar $libdir/*.jar | uniq)
do
    classpath=$classpath:$jar
done

# NOTE: the $@ must be quoted "$@" for arguments to be passed correctly
exec java -cp $classpath:$CLASSPATH PropertyChangeEventsJava_java_impl1.PropertyChangeEventsJava_java_impl1 "$@"
