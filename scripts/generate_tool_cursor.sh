#!/bin/bash

# The script creates SVG templates for all cursors.
# Before run create PNG icon of tool!
# Use SVG templates for creating PNG cursors.

# IMPORTANT!!! Run script inside 'scripts' directory.

# usage:
# $ ./generate_tool_cursor.sh

# Notes.
# No, template_cursor.svg is not broken! It is just template, no point open it.
# After creating SVG templates open them by Inkscape, previewer doesn't show pictures inside svg file linked by url.
# I have spent hour before understand why i don't see PNG inside SVG in Nautilus.

PATTERN=*@2x.png
TOOLICONPATH=../src/app/valentina/share/resources/toolicon/32x32/ # PNG tool icon should be here
TOOLICONS=`ls $TOOLICONPATH$PATTERN`
OUTPATH=../src/app/valentina/share/resources/cursor/svg # Seek SVG templates here


for var in $TOOLICONS
do
	basename=${var##*/} # remove the path from a path-string
	basename=${basename%.png} # remove the extension from a path-string
	basename=${basename%@2x} # remove optional @2x suffix
	if [ ! -f $basename@2x.png ]; then # always prefere hidpi version
		sed "s/<<basename>>/$basename@2x/" $OUTPATH/template_cursor.svg > $OUTPATH/${basename}_cursor.svg
	else
		sed "s/<<basename>>/$basename/" $OUTPATH/template_cursor.svg > $OUTPATH/${basename}_cursor.svg
	fi
done
