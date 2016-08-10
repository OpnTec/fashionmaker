#!/bin/sh
# Run this script if you want to find and update all strings in the code.
# lupdate doesn't work with recursive *.pro file and without direct pointing to correct *.pro file just update exists strings in code.
# Please, run this script from folder <root_folder>/scripts.

# Download all translations from transifex.com.
tx pull --mode=developer -f -s --skip

# Update local strings
lupdate -recursive ../share/translations/translations.pro
lupdate -recursive ../share/translations/measurements.pro
#clean stale QM files
rm -f -v ../share/translations/*.qm
# force to run qmake
MAKEFILES=`find ../../ -name Makefile`

for var in $MAKEFILES
do
    rm -f -v $var
done

echo Done.
echo For updating files run: build all.

