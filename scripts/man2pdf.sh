#!/bin/sh
# Use to conver man pages to pdf

# usage:
# $ ./man2pdf.sh

man -t ../dist/debian/valentina.1 | ps2pdf - valentina.pdf
man -t ../dist/debian/tape.1 | ps2pdf - tape.pdf
