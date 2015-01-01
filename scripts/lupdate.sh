#!/bin/sh
# Run this script if you want find and update all string in code.
# lupdate doesn't work with recursive *.pro file and without direct pointing to correct *.pro file just update exists strings in code.
# Please run this script from folder <root_folder>/scripts.
lupdate -recursive ../share/translations.pro
