#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import fnmatch
import sys
import getopt

# ./macfixqtdylibrpath.py ./Valentina.app /Users/dismine/Qt5.7.0/5.7/clang_64/bin/qmake
def usage():
  print "Usage: %s <path_to_bundle_root_directory> [qmake_path]" % os.path.basename(sys.argv[0])

def readQmakeVar(qmake_bin, var):
  pipe = os.popen(' '.join([qmake_bin, '-query', var]))
  return pipe.read().rstrip('\n')

def otool(library):
  pipe = os.popen(' '.join(['otool', '-L', library]))
  output = pipe.read().split('\n')

  i = 0
  while i < len(output):
    output[i] = output[i].replace("\t", "")
    i+=1

  i = 0
  while i < len(output):
    parts = output[i].split(" (")
    output[i] = parts[0]
    i+=1
  return output[2:]

def installNameTool(old_dependency, dLibrary, library):
  pipe = os.popen(' '.join(['install_name_tool', '-change', old_dependency, dLibrary, library]))

def findQtLibraries(filter):
  wholefiles = []
  for root, subdirs, filenames in os.walk('.'):
    for filename in filenames:
      wholefiles.append(os.path.join(root, filename))  

  filtered = []
  for filtered_file in fnmatch.filter(wholefiles, filter):
    filtered.append(filtered_file)  
  return filtered

def which(program):
  def is_exe(fpath):
    return os.path.exists(fpath) and os.access(fpath, os.X_OK)

  fpath, fname = os.path.split(program)
  if fpath:
    if is_exe(program):
      return program
  else:
    for path in os.environ["PATH"].split(os.pathsep):
      exe_file = os.path.join(path, program)
      if is_exe(exe_file):
        return exe_file

  return None

def fixLibrary(libraries, QT_INSTALL_LIBS):
  for filtered_file in libraries:
    print "Library: " + filtered_file
    dependencies = otool(filtered_file)

    for dependency in dependencies:
      if dependency.startswith(QT_INSTALL_LIBS):
        library = dependency.replace(QT_INSTALL_LIBS, "")
        print "install_name_tool -change " + "%s " % dependency + "@rpath%s " % library + filtered_file
        installNameTool(dependency, "@rpath%s" % library, filtered_file)

def fixFrameworks(QT_INSTALL_LIBS):
  filtered = findQtLibraries('*/Qt*.framework/Versions/*/Qt*')
  fixLibrary(filtered, QT_INSTALL_LIBS)

def fixDylibs(QT_INSTALL_LIBS):
  filtered = findQtLibraries('*/*.dylib')
  fixLibrary(filtered, QT_INSTALL_LIBS)

def main():
  try:
    opts, args = getopt.gnu_getopt(sys.argv[1:], 'h', ['help'])
  except:
    usage()
    sys.exit(2)
  for o, a in opts:
    if o in ('-h', '--help'):
      usage()
      sys.exit(0)

  if len(args) < 1:
    usage()
    sys.exit(2)

  os.chdir(args[0] + "/Contents/")
  print "Current directory: " + os.getcwd()

  qmake_bin = 'qmake'
  if len(args) > 1:
    qmake_bin = args[1]
  qmake_bin = which(qmake_bin)

  if qmake_bin == None:
    print "Cannot find required binary 'qmake'."
    sys.exit(2)

  print "qmake path: " + qmake_bin

  QT_INSTALL_LIBS = readQmakeVar(qmake_bin, 'QT_INSTALL_LIBS')
  print "Path to Qt libraries: " + QT_INSTALL_LIBS 

  fixFrameworks(QT_INSTALL_LIBS)
  fixDylibs(QT_INSTALL_LIBS)

if __name__ == "__main__":
    if sys.platform != 'darwin':
        print "This script supports only Mac OS."
        sys.exit(2)
    else:
        main()
