#!/bin/sh
# Run this script if you want to find and update all strings in the code.
# lupdate doesn't work with recursive *.pro file and without direct pointing to correct *.pro file just update exists strings in code.
# Please, run this script from folder <root_folder>/scripts.

# Download last translations from transifex.com. 
tx pull --mode=developer

# Update local strings
lupdate -recursive ../share/translations/translations.pro
lupdate -recursive ../share/translations/measurements.pro

rm -f -v ../share/translations/*.qm
