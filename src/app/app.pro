#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:36:43
#
#-------------------------------------------------

# Use out-of-source builds (shadow builds)

include(../../Valentina.pri)

QT       += core gui widgets xml svg printsupport xmlpatterns

TEMPLATE = app

TARGET = valentina

CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++11

#DEFINES += ...

# directory for executable file
DESTDIR = bin

# files created moc
MOC_DIR = moc

# objecs files
OBJECTS_DIR = obj

# files created rcc
RCC_DIR = rcc

# files created uic
UI_DIR = uic

include(container/container.pri)
include(dialogs/dialogs.pri)
include(exception/exception.pri)
include(geometry/geometry.pri)
include(tools/tools.pri)
include(widgets/widgets.pri)
include(xml/xml.pri)
include(undocommands/undocommands.pri)
include(visualization/visualization.pri)

INCLUDEPATH += "$${PWD}/widgets"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        tablewindow.cpp \
        stable.cpp \
        version.cpp \
        options.cpp

HEADERS  += \
        mainwindow.h \
        options.h \
        tablewindow.h \
        stable.h \
        version.h

FORMS    += \
        mainwindow.ui \
        tablewindow.ui

RESOURCES += \
    share/resources/icon.qrc \
    share/resources/cursor.qrc \
    share/resources/theme.qrc \
    share/resources/schema.qrc \
    share/resources/measurements.qrc

OTHER_FILES += \
    share/resources/valentina.rc \
    share/resources/valentina.exe.manifest \
    share/resources/icon/64x64/icon64x64.ico

TRANSLATIONS += share/translations/valentina.ts \
                share/translations/valentina_ru_RU.ts \
                share/translations/valentina_uk_UA.ts \
                share/translations/valentina_de_DE.ts \
                share/translations/valentina_cs_CZ.ts \
                share/translations/valentina_he_IL.ts \
                share/translations/valentina_fr_FR.ts \
                share/translations/valentina_it_IT.ts \
                share/translations/valentina_nl_NL.ts

unix {
    *-g++{
        QMAKE_CXX = ccache g++
    }
    clang*{
        QMAKE_CXX = ccache clang++
    }
}

CONFIG += precompile_header
# Precompiled headers (PCH)
PRECOMPILED_HEADER = stable.h
win32-msvc* {
    PRECOMPILED_SOURCE = stable.cpp
}

CONFIG(debug, debug|release){
    # Debug
    unix {
        *-g++{
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            -O0 \
            -Wall \
            -Wextra \
            -pedantic \
            -Weffc++ \
            -Woverloaded-virtual \
            -Wctor-dtor-privacy \
            -Wnon-virtual-dtor \
            -Wold-style-cast \
            -Wconversion \
            -Winit-self \
            -Wstack-protector \
            -Wunreachable-code \
            -Wcast-align \
            -Wcast-qual \
            -Wdisabled-optimization \
            -Wfloat-equal \
            -Wformat=2 \
            -Wimport \
            -Wmissing-include-dirs \
            -Wpacked \
            -Wredundant-decls \
            -Winline \
            -Winvalid-pch \
            -Wunsafe-loop-optimizations \
            -Wlong-long \
            -Wmissing-format-attribute \
            -Wswitch-default \
            -Wuninitialized \
            -Wvariadic-macros \
            -Wlogical-op \
            -Wnoexcept \
            -Wmissing-noreturn \
            -Wpointer-arith \
            -Wstrict-null-sentinel \
            -Wstrict-overflow=5 \
            -Wundef \
            -Wno-unused \
            -gdwarf-3 \
            -ftrapv
        }
        #Turn on Clang warnings
        clang*{
        QMAKE_CXXFLAGS += \
            -isystem "/usr/include/qt5" \
            -isystem "/usr/include/qt5/QtWidgets" \
            -isystem "/usr/include/qt5/QtXml" \
            -isystem "/usr/include/qt5/QtGui" \
            -isystem "/usr/include/qt5/QtCore" \
            -isystem "/usr/include/qt5/QtXmlPatterns" \
            -isystem "$${OUT_PWD}/$${UI_DIR}" \
            -isystem "$${OUT_PWD}/$${MOC_DIR}" \
            -isystem "$${OUT_PWD}/$${RCC_DIR}" \
            -O0 \
            -fparse-all-comments \
            -Wabi \
            -Wabstract-final-class \
            -Wabstract-vbase-init \
            -Waddress \
            -Waddress-of-array-temporary \
            -Waddress-of-temporary \
            -Waggregate-return \
            -Wall \
            -Wambiguous-macro \
            -Wambiguous-member-template \
            -Wanalyzer-incompatible-plugin \
            -Wanonymous-pack-parens \
            -Warc \
            -Warc-abi \
            -Warc-bridge-casts-disallowed-in-nonarc \
            -Warc-maybe-repeated-use-of-weak \
            -Warc-non-pod-memaccess \
            -Warc-performSelector-leaks \
            -Warc-repeated-use-of-weak \
            -Warc-retain-cycles \
            -Warc-unsafe-retained-assign \
            -Warray-bounds \
            -Warray-bounds-pointer-arithmetic \
            -Wasm \
            -Wasm-operand-widths \
            -Wassign-enum \
            -Watomic-properties \
            -Watomic-property-with-user-defined-accessor \
            -Wattributes \
            -Wauto-import \
            -Wauto-storage-class \
            -Wauto-var-id \
            -Wavailability \
            -Wbackslash-newline-escape \
            -Wbad-array-new-length \
            -Wbad-function-cast \
            -Wbind-to-temporary-copy \
            -Wbitfield-constant-conversion \
            -Wbitwise-op-parentheses \
            -Wbool-conversion \
            -Wbool-conversions \
            -Wbuiltin-macro-redefined \
            -Wbuiltin-requires-header \
            -Wc++-compat \
            -Wc++0x-compat \
            -Wc++0x-extensions \
            -Wc++0x-narrowing \
            -Wc++11-compat \
            -Wc++11-compat-pedantic \
            -Wc++11-compat-reserved-user-defined-literal \
            -Wc++11-extensions \
            -Wc++11-extra-semi \
            -Wc++11-long-long \
            -Wc++11-narrowing \
            -Wc11-extensions \
            -Wcast-align \
            -Wcast-of-sel-type \
            -Wcast-qual \
            -WCFString-literal \
            -Wchar-align \
            -Wchar-subscripts \
            -Wcomment \
            -Wcomments \
            -Wcompare-distinct-pointer-types \
            -Wcomplex-component-init \
            -Wconditional-type-mismatch \
            -Wconditional-uninitialized \
            -Wconfig-macros \
            -Wconstant-conversion \
            -Wconstant-logical-operand \
            -Wconstexpr-not-const \
            -Wconversion \
            -Wconversion-null \
            -Wno-covered-switch-default \
            -Wctor-dtor-privacy \
            -Wcustom-atomic-properties \
            -Wdangling-else \
            -Wdangling-field \
            -Wdangling-initializer-list \
            -Wdeclaration-after-statement \
            -Wdelegating-ctor-cycles \
            -Wdelete-incomplete \
            -Wdelete-non-virtual-dtor \
            -Wdeprecated \
            -Wdeprecated-declarations \
            -Wdeprecated-implementations \
            -Wdeprecated-objc-isa-usage \
            -Wdeprecated-objc-pointer-introspection \
            -Wdeprecated-objc-pointer-introspection-performSelector \
            -Wdeprecated-register \
            -Wdeprecated-writable-strings \
            -Wdirect-ivar-access \
            -Wdisabled-macro-expansion \
            -Wdisabled-optimization \
            -Wdiscard-qual \
            -Wdistributed-object-modifiers \
            -Wdiv-by-zero \
            -Wdivision-by-zero \
            -Wdocumentation \
            -Wdocumentation-deprecated-sync \
            -Wdocumentation-html \
            -Wdollar-in-identifier-extension \
            -Wduplicate-decl-specifier \
            -Wduplicate-enum \
            -Wduplicate-method-arg \
            -Wduplicate-method-match \
            -Wdynamic-class-memaccess \
            -Weffc++ \
            -Wembedded-directive \
            -Wempty-body \
            -Wempty-translation-unit \
            -Wendif-labels \
            -Wenum-compare \
            -Wenum-conversion \
            -Wexplicit-ownership-type \
            -Wextended-offsetof \
            -Wextern-initializer \
            -Wextra \
            -Wextra-semi \
            -Wextra-tokens \
            -Wflexible-array-extensions \
            -Wfloat-equal \
            -Wformat \
            -Wformat-extra-args \
            -Wformat-invalid-specifier \
            -Wformat-non-iso \
            -Wformat-nonliteral \
            -Wformat-security \
            -Wformat-y2k \
            -Wformat-zero-length \
            -Wformat=2 \
            -Wfour-char-constants \
            -Wgcc-compat \
            -Wgnu \
            -Wgnu-array-member-paren-init \
            -Wgnu-designator \
            -Wgnu-static-float-init \
            -Wheader-guard \
            -Wheader-hygiene \
            -Widiomatic-parentheses \
            -Wignored-attributes \
            -Wignored-qualifiers \
            -Wimplicit \
            -Wimplicit-atomic-properties \
            -Wimplicit-conversion-floating-point-to-bool \
            -Wimplicit-exception-spec-mismatch \
            -Wimplicit-fallthrough \
            -Wimplicit-fallthrough-per-function \
            -Wimplicit-function-declaration \
            -Wimplicit-int \
            -Wimplicit-retain-self \
            -Wimport \
            -Wimport-preprocessor-directive-pedantic \
            -Wincompatible-library-redeclaration \
            -Wincompatible-pointer-types \
            -Wincompatible-pointer-types-discards-qualifiers \
            -Wincomplete-implementation \
            -Wincomplete-module \
            -Wincomplete-umbrella \
            -Winherited-variadic-ctor \
            -Winit-self \
            -Winitializer-overrides \
            -Winline \
            -Wint-conversion \
            -Wint-conversions \
            -Wint-to-pointer-cast \
            -Wint-to-void-pointer-cast \
            -Winteger-overflow \
            -Winvalid-constexpr \
            -Winvalid-iboutlet \
            -Winvalid-noreturn \
            -Winvalid-offsetof \
            -Winvalid-pch \
            -Winvalid-pp-token \
            -Winvalid-source-encoding \
            -Winvalid-token-paste \
            -Wknr-promoted-parameter \
            -Wlanguage-extension-token \
            -Wlarge-by-value-copy \
            -Wliteral-conversion \
            -Wliteral-range \
            -Wlocal-type-template-args \
            -Wlogical-not-parentheses \
            -Wlogical-op-parentheses \
            -Wlong-long \
            -Wloop-analysis \
            -Wmain \
            -Wmain-return-type \
            -Wmalformed-warning-check \
            -Wmethod-signatures \
            -Wmicrosoft \
            -Wmicrosoft-exists \
            -Wmismatched-parameter-types \
            -Wmismatched-return-types \
            -Wmismatched-tags \
            -Wmissing-braces \
            -Wmissing-declarations \
            -Wmissing-field-initializers \
            -Wmissing-format-attribute \
            -Wmissing-include-dirs \
            -Wmissing-method-return-type \
            -Wmissing-noreturn \
            -Wmissing-prototypes \
            -Wmissing-selector-name \
            -Wmissing-sysroot \
            -Wmissing-variable-declarations \
            -Wmodule-conflict \
            -Wmost \
            -Wmultichar \
            -Wnarrowing \
            -Wnested-externs \
            -Wnewline-eof \
            -Wnon-gcc \
            -Wnon-literal-null-conversion \
            -Wnon-pod-varargs \
            -Wnon-virtual-dtor \
            -Wnonnull \
            -Wnonportable-cfstrings \
            -WNSObject-attribute \
            -Wnull-arithmetic \
            -Wnull-character \
            -Wnull-conversion \
            -Wnull-dereference \
            -Wodr \
            -Wold-style-cast \
            -Wold-style-definition \
            -Wout-of-line-declaration \
            -Wover-aligned \
            -Woverflow \
            -Woverlength-strings \
            -Woverloaded-shift-op-parentheses \
            -Woverloaded-virtual \
            -Woverriding-method-mismatch \
            -Wpacked \
            -Wparentheses \
            -Wparentheses-equality \
            -Wpedantic \
            -Wpointer-arith \
            -Wpointer-sign \
            -Wpointer-to-int-cast \
            -Wpointer-type-mismatch \
            -Wpredefined-identifier-outside-function \
            -Wprivate-extern \
            -Wprotocol \
            -Wprotocol-property-synthesis-ambiguity \
            -Wreadonly-iboutlet-property \
            -Wreceiver-expr \
            -Wreceiver-forward-class \
            -Wreceiver-is-weak \
            -Wredundant-decls \
            -Wreinterpret-base-class \
            -Wreorder \
            -Wrequires-super-attribute \
            -Wreserved-user-defined-literal \
            -Wreturn-stack-address \
            -Wreturn-type \
            -Wreturn-type-c-linkage \
            -Wsection \
            -Wselector \
            -Wselector-type-mismatch \
            -Wself-assign \
            -Wself-assign-field \
            -Wsemicolon-before-method-body \
            -Wsentinel \
            -Wsequence-point \
            -Wserialized-diagnostics \
            -Wshadow-ivar \
            -Wshift-count-negative \
            -Wshift-count-overflow \
            -Wshift-op-parentheses \
            -Wshift-overflow \
            -Wshift-sign-overflow \
            -Wshorten-64-to-32 \
            -Wsign-compare \
            -Wsign-conversion \
            -Wsign-promo \
            -Wsizeof-array-argument \
            -Wsizeof-array-decay \
            -Wsizeof-pointer-memaccess \
            -Wsometimes-uninitialized \
            -Wsource-uses-openmp \
            -Wstack-protector \
            -Wstatic-float-init \
            -Wstatic-in-inline \
            -Wstatic-local-in-inline \
            -Wstatic-self-init \
            -Wstrict-aliasing \
            -Wstrict-aliasing=0 \
            -Wstrict-aliasing=1 \
            -Wstrict-aliasing=2 \
            -Wstrict-overflow \
            -Wstrict-overflow=0 \
            -Wstrict-overflow=1 \
            -Wstrict-overflow=2 \
            -Wstrict-overflow=3 \
            -Wstrict-overflow=4 \
            -Wstrict-overflow=5 \
            -Wstrict-prototypes \
            -Wstrict-selector-match \
            -Wstring-compare \
            -Wstring-conversion \
            -Wstring-plus-int \
            -Wstrlcpy-strlcat-size \
            -Wstrncat-size \
            -Wsuper-class-method-mismatch \
            -Wswitch \
            -Wswitch-default \
            -Wswitch-enum \
            -Wsynth \
            -Wtautological-compare \
            -Wtautological-constant-out-of-range-compare \
            -Wtentative-definition-incomplete-type \
            -Wthread-safety \
            -Wthread-safety-analysis \
            -Wthread-safety-attributes \
            -Wthread-safety-beta \
            -Wthread-safety-precise \
            -Wtrigraphs \
            -Wtype-limits \
            -Wtype-safety \
            -Wtypedef-redefinition \
            -Wtypename-missing \
            -Wundeclared-selector \
            -Wundef \
            -Wundefined-inline \
            -Wundefined-internal \
            -Wundefined-reinterpret-cast \
            -Wunicode \
            -Wunicode-whitespace \
            -Wuninitialized \
            -Wunknown-warning-option \
            -Wunnamed-type-template-args \
            -Wunneeded-internal-declaration \
            -Wunneeded-member-function \
            -Wunsequenced \
            -Wunsupported-visibility \
            -Wunused \
            -Wunused-argument \
            -Wunused-command-line-argument \
            -Wunused-comparison \
            -Wunused-exception-parameter \
            -Wunused-function \
            -Wunused-label \
            -Wunused-member-function \
            -Wunused-parameter \
            -Wunused-private-field \
            -Wunused-result \
            -Wunused-sanitize-argument \
            -Wunused-value \
            -Wunused-variable \
            -Wunused-volatile-lvalue \
            -Wuser-defined-literals \
            -Wvarargs \
            -Wvariadic-macros \
            -Wvector-conversion \
            -Wvector-conversions \
            -Wvexing-parse \
            -Wvisibility \
            -Wvla \
            -Wvla-extension \
            -Wvolatile-register-var \
            -Wweak-template-vtables \
            -Wweak-vtables \
            -Wwrite-strings \
            -Wzero-length-array
        }
    } else {
        *-g++{#Don't use additional GCC keys on Windows system.
        QMAKE_CXXFLAGS += -O0 -Wall -Wextra -pedantic
        }
    }

    #Calculate revision number only in release mode. Change revision number each time requare recompilation
    #precompiled headers file.
    DEFINES += "LOC_REV=0"
}else{
    # Release
    *-g++{
    QMAKE_CXXFLAGS += -O2
    }

    DEFINES += QT_NO_DEBUG_OUTPUT

    #local revision number for using in version
    HG_REV=$$system(hg parents --template '{rev}')
    isEmpty(HG_REV){
        HG_REV = 0
    }
    DEFINES += "LOC_REV=$${HG_REV}"
}

message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation: $$[QT_INSTALL_DOCS])
message(Header files: $$[QT_INSTALL_HEADERS])
message(Libraries: $$[QT_INSTALL_LIBS])
message(Binary files (executables): $$[QT_INSTALL_BINS])
message(Plugins: $$[QT_INSTALL_PLUGINS])
message(Data files: $$[QT_INSTALL_DATA])
message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
message(Settings: $$[QT_INSTALL_SETTINGS])
message(Examples: $$[QT_INSTALL_EXAMPLES])

win32:RC_FILE = share/resources/valentina.rc

INSTALL_TRANSLATIONS += \
    share/translations/valentina_ru_RU.qm \
    share/translations/valentina_uk_UA.qm \
    share/translations/valentina_de_DE.qm \
    share/translations/valentina_cs_CZ.qm \
    share/translations/valentina_he_IL.qm \
    share/translations/valentina_fr_FR.qm \
    share/translations/valentina_it_IT.qm \
    share/translations/valentina_nl_NL.qm

INSTALL_STANDARD_MEASHUREMENTS += share/resources/tables/standard/GOST_man_ru.vst

unix {
#VARIABLES
isEmpty(PREFIX) {
  PREFIX = /usr
}
BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share
DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
#MAKE INSTALL
target.path = $$BINDIR
desktop.path = $$DATADIR/applications/
desktop.files += ../../dist/$${TARGET}.desktop
pixmaps.path = $$DATADIR/pixmaps/
pixmaps.files += ../../dist/$${TARGET}.png
translations.path = $$DATADIR/$${TARGET}/translations/
translations.files = $$INSTALL_TRANSLATIONS
standard.path = $$DATADIR/$${TARGET}/tables/standard/
standard.files = $$INSTALL_STANDARD_MEASHUREMENTS
INSTALLS += \
    target \
    desktop \
    pixmaps \
    translations \
    standard
}

!isEmpty(TRANSLATIONS): {
    for(_translation_name, TRANSLATIONS) {
      _translation_name_qm = $$section(_translation_name,".", 0, 0).qm

      system($$shell_path($$[QT_INSTALL_BINS]/lrelease) $$shell_path($${PWD}/$$_translation_name) -qm $$shell_path($${PWD}/$$_translation_name_qm))
    }
}

for(DIR, INSTALL_TRANSLATIONS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     tr_path += $${PWD}/$$DIR
}

copyToDestdir($$tr_path, $$shell_path($${OUT_PWD}/$$DESTDIR/translations))

for(DIR, INSTALL_STANDARD_MEASHUREMENTS) {
     #add these absolute paths to a variable which
     #ends up as 'mkcommands = path1 path2 path3 ...'
     st_path += $${PWD}/$$DIR
}

copyToDestdir($$st_path, $$shell_path($${OUT_PWD}/$$DESTDIR/tables/standard))

win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser2
else:unix: LIBS += -L$${OUT_PWD}/../libs/qmuparser/$${DESTDIR} -lqmuparser

INCLUDEPATH += $${PWD}/../libs/qmuparser
DEPENDPATH += $${PWD}/../libs/qmuparser

win32:CONFIG(release, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer
else:unix: LIBS += -L$${OUT_PWD}/../libs/vpropertyexplorer/$${DESTDIR} -lvpropertyexplorer

INCLUDEPATH += $${PWD}/../libs/vpropertyexplorer
DEPENDPATH += $${PWD}/../libs/vpropertyexplorer
