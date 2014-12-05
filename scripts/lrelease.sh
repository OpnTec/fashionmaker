#!/bin/sh
# Run this script if you want release all translated string in program.
# lrelease doesn't work with recursive *.pro file and without direct pointing to correct *.pro doesn't find anything.
# Please run this script from folder <root_folder>/scripts. 
lrelease -removeidentical -nounfinished ../share/translations.pro
