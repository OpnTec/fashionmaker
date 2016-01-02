#!/bin/bash

# example:
# $ ./generate_tool_cursor.sh ../../toolicon/32x32/*@2x.png

for var in "$@"
do
	basename=${var##*/}
	basename=${basename%.png}
	basename=${basename%@2x} # remove optional @2x suffix
	if [ ! -f $basename@2x.png ]; then # always prefere hidpi version
		sed "s/<<basename>>/$basename@2x/" template_cursor.svg > ${basename}_cursor.svg
	else
		sed "s/<<basename>>/$basename/" template_cursor.svg > ${basename}_cursor.svg
	fi
done
