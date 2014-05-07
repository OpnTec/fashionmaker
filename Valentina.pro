include(Valentina.pri)

#version check qt
!minQtVersion(5, 1, 0) {
    message("Cannot build Valentina with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.1.0.")
}

TEMPLATE = subdirs
SUBDIRS = src
