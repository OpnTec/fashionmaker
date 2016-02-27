#!/bin/sh
# Prepare files from dist/debian/* for openSUSE Build Service.
# Use if need to update files.
# Please, run this script from folder <root_folder>/scripts.

# usage:
# $ ./generate_tool_cursor.sh

# Note. For stable version enough create an archive debian.tar.gz with folder debian inside and download it.

ORIGDEBIAN=../dist/debian/ # Path to original files
OBSDEBIAN=../dist/OBS_debian # Path to prepared files
FILES=`find $ORIGDEBIAN -type f | xargs ls -1`

for file in $FILES
do
	basename=${file##*/} # remove the path from a path-string
	cp -v -f $file $OBSDEBIAN/debian.${basename}	
done
