#!/bin/bash

# usage:
# $ ./generate_tool_cursor.sh

PATTERN=*@2x.png
TOOLICONPATH=../src/app/valentina/share/resources/toolicon/32x32/
TOOLICONS=`ls $TOOLICONPATH$PATTERN`
OUTPATH=../src/app/valentina/share/resources/cursor/svg


for var in $TOOLICONS
do
	basename=${var##*/}
	basename=${basename%.png}
	basename=${basename%@2x} # remove optional @2x suffix
	if [ ! -f $basename@2x.png ]; then # always prefere hidpi version
		sed "s/<<basename>>/$basename@2x/" $OUTPATH/template_cursor.svg > $OUTPATH/${basename}_cursor.svg
	else
		sed "s/<<basename>>/$basename/" $OUTPATH/template_cursor.svg > $OUTPATH/${basename}_cursor.svg
	fi
done
