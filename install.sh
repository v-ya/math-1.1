#!/bin/sh
cp math /usr/bin/math
if [ -d /usr/share/gtksourceview-3.0/language-specs/ ] ; then
	cp doc/math.lang /usr/share/gtksourceview-3.0/language-specs/math.lang
fi

rm -Rf math-root/
mkdir math-root
mkdir math-root/include math-root/import math-root/data
cp include/* math-root/include/
cp package/model/* math-root/import/

