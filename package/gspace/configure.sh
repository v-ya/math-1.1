#!/bin/sh

gl_version=$(glxinfo | grep "Max core profile version" | awk '{split($0, a, ": ");split(a[2], a, ".");print a[1], a[2];}');
gl_major=$(echo $gl_version | awk '{print $1;}')
gl_minor=$(echo $gl_version | awk '{print $2;}')

echo "// This is auto builded file"
echo "#define _sys_opengl_version_major $gl_major"
echo "#define _sys_opengl_version_minor $gl_minor"

