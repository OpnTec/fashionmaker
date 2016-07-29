#Turn on compilers warnings.
unix {
    *-g++{
        QMAKE_CXXFLAGS += \
            $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }

        noAddressSanitizer{ # For enable run qmake with CONFIG+=noAddressSanitizer
            # do nothing
        } else {
            #gcc’s 4.8.0 Address Sanitizer
            #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
            QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
            QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
            QMAKE_LFLAGS += -fsanitize=address
        }
    }

    clang*{
        QMAKE_CXXFLAGS += \
            $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }

        # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
        # want them in global list. Compromise decision delete them from local list.
        QMAKE_CXXFLAGS -= \
            -Wmissing-prototypes
    }

    *-icc-*{
        QMAKE_CXXFLAGS += \
            $$ICC_DEBUG_CXXFLAGS

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }
    }
} else { # Windows
    *-g++{
        QMAKE_CXXFLAGS += $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }
    }

    win32-msvc*{
        QMAKE_CXXFLAGS += $$MSVC_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            #QMAKE_CXXFLAGS += -WX
        }
    }
}
