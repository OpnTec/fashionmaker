#!/bin/bash

# The script helps to automate fixing issue "libpng warning: iCCP: known incorrect sRGB profile".
# Main idea: https://stackoverflow.com/questions/22745076/libpng-warning-iccp-known-incorrect-srgb-profile

# Use "pngcrush" to remove the incorrect sRGB profile from the png file
# Link: http://pmt.sourceforge.net/pngcrush/

# -ow will overwrite the input file
# -rem allb will remove all ancillary chunks except tRNS and gAMA
# -reduce does lossless color-type or bit-depth reduction

#In the console output you should see "Removed the sRGB chunk.", and possibly more messages about chunk removals. 
#You will end up with a smaller, optimized png file. As the command will overwrite the original file, make 
#sure to create a backup or use version control.

# IMPORTANT!!! Run script inside 'scripts' directory.

# usage:
# $ ./fix_png.sh

find .. -type f -iname '*.png' -exec pngcrush -ow -rem allb -reduce {} \;
