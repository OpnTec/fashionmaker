#Turn on compilers warnings.
unix {
    *-g++{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }

        noAddressSanitizer{ # For enable run qmake with CONFIG+=noAddressSanitizer
            # do nothing
        } else {
            CONFIG(debug, debug|release){
                # Debug mode
                #gcc’s 4.8.0 Address Sanitizer
                #http://blog.qt.digia.com/blog/2013/04/17/using-gccs-4-8-0-address-sanitizer-with-qt/
                QMAKE_CXXFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
                QMAKE_LFLAGS += -fsanitize=address
            }
        }
    }

    clang*{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }
    }

    *-icc-*{
        QMAKE_CXXFLAGS += \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
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
            QMAKE_CXXFLAGS += -WX
        }
    }
}
