#Turn on compilers warnings.
unix {
    *-g++{
        QMAKE_CXXFLAGS += \
            # Key -isystem disable checking errors in system headers.
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            $$GCC_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }

        # -isystem key works only for headers. In some cases it's not enough. But we can't delete these warnings and
        # want them in the global list. Compromise decision is to delete them from the local list.
        QMAKE_CXXFLAGS -= \
            -Wlong-long \

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
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            $$CLANG_DEBUG_CXXFLAGS # See common.pri for more details.

        checkWarnings{ # For enable run qmake with CONFIG+=checkWarnings
            QMAKE_CXXFLAGS += -Werror
        }

        # -isystem key works only for headers. In some cases it's not enough. But we can't delete this warnings and
        # want them in global list. Compromise decision delete them from local list.
        QMAKE_CXXFLAGS -= \
            -Wundefined-reinterpret-cast \
            -Wmissing-prototypes # rcc folder
    }

    *-icc-*{
        QMAKE_CXXFLAGS += \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
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
