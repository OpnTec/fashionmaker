TEMPLATE = subdirs
#SUBDIRS = sub_lib sub_tests sub_app
SUBDIRS = \
    sub_app \
    sub_lib_qmuparser

sub_lib_qmuparser.subdir = libs/qmuparser
sub_lib_qmuparser.file = libs/qmuparser/qmuparser.pro
#sub_tests.file = tests/proj.pro
#sub_tests.depends = sub_lib
sub_app.subdir = app
sub_app.file = app/app.pro
sub_app.depends = sub_lib_qmuparser

#This makes it possible to use make -j 4 on your fancy quad-core system with a project that consists of several
#components that depend on each other. To simplify the process a bit, the following test function can be defined:

# addSubdirs(subdirs,deps): Adds directories to the project that depend on
# other directories
defineTest(addSubdirs) {
    for(subdirs, 1) {
        entries = $$files($$subdirs)
        for(entry, entries) {
            name = $$replace(entry, [/\\\\], _)
            SUBDIRS += $$name
            eval ($${name}.subdir = $$entry)
            for(dep, 2):eval ($${name}.depends += $$replace(dep, [/\\\\], _))
            export ($${name}.subdir)
            export ($${name}.depends)
        }
    }
    export (SUBDIRS)
}
#You can then use it like to define a project that has:

#several contributed modules that should be compiled first
#addSubdirs (contrib/*)

#a kernel lib for non-gui related stuff that depends on some contrib modules
#addSubdirs (src/lib/kernel, contrib/module1 contrib/module2)

#a gui lib that depends on the kernel lib and some other contrib modules
#addSubdirs (src/lib/gui, src/lib/kernel contrib/module3 contrib/module4)

#test benches for the kernel and gui libs
#addSubdirs (src/tests/kernel, src/lib/kernel)
#addSubdirs (src/tests/gui, src/lib/gui)

#a main program that uses the gui and kernel libs
#addSubdirs (src/main, src/lib/gui src/lib/kernel)

#several modules that only depend on the kernel lib
#addSubdirs (src/modules/*, src/lib/kernel)
