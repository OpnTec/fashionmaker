include(common.pri)

#Check if Qt version >= 5.0.0
!minQtVersion(5, 0, 0) {
    message("Cannot build Valentina with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.0.0.")
}

#These checks need because we can quote all paths that contain space.
LIST = $$split(PWD,' ')
count(LIST, 1, >): error("The build will fail. Path '$${PWD}' contains space!!!")

LIST = $$split(OUT_PWD,' ')
count(LIST, 1, >): error("The build will fail. Path '$${OUT_PWD}' contains space!!!")

unix {
    *-g++ {
        GCC_VERSION = $$system("g++ -dumpversion")
        contains(GCC_VERSION, 6.[0-9]) {
            message( "g++ version 6.x found" )
            CONFIG += g++6
        } else {
            contains(GCC_VERSION, 5.[0-9]) {
                message( "g++ version 5.x found" )
                CONFIG += g++5
            } else {
                contains(GCC_VERSION, 4.[0-9]) {
                    message( "g++ version 4.x found" )
                    CONFIG += g++4
                } else {
                    message( "Unknown GCC configuration" )
                }
            }
        }
    }
}

TEMPLATE = subdirs
SUBDIRS = src
