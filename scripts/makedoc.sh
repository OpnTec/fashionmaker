#!/bin/sh
# Script generate developer documentation for whole progect.
# Output directories src/app/doc/, src/libs/qmuparser/doc/
# Run from "scripts" directory
cd ..
cd src/app
doxygen doc/doxygen/Doxyfile
cd ..
cd libs/qmuparser
doxygen doc/doxygen/Doxyfile
