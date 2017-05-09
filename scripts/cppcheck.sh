#!/bin/sh
# This script helps run cppcheck with the same keys we have on codeship.com (except for key --platform=unix32).
# Please, run this script from folder <root_folder>/scripts.

# Because we use the last available cppcheck version usually we build it manually. 
CPPCHECK="../../../../cppcheck/cppcheck"
$CPPCHECK \
          -j4 -f -q \
          -U__INTEL_COMPILER_UPDATE \
          -UqApp \
          --template '{file}:{line}:{message}:{id}' \
          --inline-suppr \
          --platform=unix32 \
          --std=c++11 \
          --std=posix \
          --enable=all \
          --library=qt.cfg \
          --library=std.cfg \
          --library=posix.cfg \
          --inconclusive \
          --suppress=leakReturnValNotUsed:../src/app/valentina/core/vapplication.cpp \
          --suppress=unmatchedSuppression:../src/libs/ifc/xml/vdomdocument.cpp \
          --suppress=redundantCondition:../src/libs/vtools/tools/drawTools/toolcurve/vtoolsplinepath.cpp \
          --suppress=unmatchedSuppression:../src/libs/vtools/tools/drawTools/toolcurve/vtoolsplinepath.cpp \
          ../src
