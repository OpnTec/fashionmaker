include(Valentina.pri)

#Check if Qt version >= 5.2.1
!minQtVersion(5, 2, 1) {
    message("Cannot build Valentina with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.2.1.")
}

TEMPLATE = subdirs
SUBDIRS = src
