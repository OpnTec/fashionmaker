#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T17:27:03
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_test

# CONFIG += testcase adds a  'make check' which is great. But by default it also
# adds a 'make install' that installs the test cases, which we do not want.
# Can configure it not to do that with 'no_testcase_installs'
# We use C++11 standard
CONFIG += c++11 testcase no_testcase_installs

# Use out-of-source builds (shadow builds)
CONFIG -= app_bundle debug_and_release debug_and_release_target

TEMPLATE = app

# directory for executable file
DESTDIR = bin

# Directory for files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    qttestmainlambda.cpp \
    tst_test.cpp

HEADERS += \
    tst_test.h
