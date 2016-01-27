#!/bin/sh
# Helps to run Clang Static Analyzer.
# Please, run this script from folder <root_folder>/scripts.
cd ..
rm -rfv build
mkdir build
cd build
qmake ../Valentina.pro -r -spec linux-clang
scan-build --use-cc clang --use-c++ clang++ make -j4
