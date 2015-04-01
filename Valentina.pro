include(Valentina.pri)

#Check if Qt version >= 5.0.0
!minQtVersion(5, 0, 0) {
    message("Cannot build Valentina with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.0.0.")
}

TEMPLATE = subdirs
SUBDIRS = src
