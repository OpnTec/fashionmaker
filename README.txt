Pattern making program
Author Roman Telezhynskyi <dismine(at)gmail.com>
Copyright (C) 2013-2015 Valentina project  
Valentina Web page: http://www.valentina-project.org/ 
Wiki: https://wiki.valentinaproject.org   
Valentina user manual https://bitbucket.org/dismine/valentina/wiki/manual/Content    
Valentina main repository: https://bitbucket.org/dismine/valentina/overview    
Forum http://forum.valentina-project.org/    
PPA for Ubuntu (develop): https://launchpad.net/~dismine/+archive/valentina-dev    
Help translate https://www.transifex.com/organization/valentina    
IRC channel: #valentina on irc.freenode.net.

Valentina
==========
Valentina is a cross-platform patternmaking program which allows designers 
to create and model patterns of clothing. This software allows pattern 
creation, using either standard sizing tables or an individual’s set of 
measurements. It blends new technologies with traditional methods to create 
a unique pattern making tool. 

Supported Platforms
===================
The standalone binary packages support the following platforms:

Windows XP SP2 (32-bit) or later
Ubuntu Linux 14.04 (32-bit/64-bit) or later
OpenSUSE 13.02 (32-bit/64-bit) or later
Fedora 20 (32-bit/64-bit) or later
Mac OS X 10.7 (64-bit) or later

Building the sources requires Qt 5.2.0 or later.

Compiling Valentina
====================
Prerequisites:   
   * Qt 5.2.0 or later (On Unix development packages needed)   
   * mercurial (only for working with repository)   
   * On Unix:     
     - g++ (at least GCC 4.6 is needed and GCC 4.8 is recommended) or
       clang (clang 3.4 recommended)
     - xpdf package (tool pdftops).    
   * On Windows:   
     - MinGW  
     - Xpdf is an open source viewer for Portable Document Format (PDF) 
     files. Website http://www.foolabs.com/xpdf/. Put tool pdftops.exe 
     in the same directory with Valentina's binary file.

The installed toolchains have to match the one Qt was compiled with.

You can build Valentina with

    cd $SOURCE_DIRECTORY\build
    qmake ..\Valentina.pro -r CONFIG+=noDebugSymbols CONFIG+=no_ccache
    make (or mingw32-make or nmake or jom, depending on your platform)

Note: On some platforms e.g. Fedora, all Qt-tools come as qmake-qt5 
    etc. In this case you should install qtchooser. This wrapper 
    resolves the Qt version with respect to QT_SELECT. So run 

    export QT_SELECT=5

    prior to qmake. You will not come along with just using qmake-qt5
    here.


Note:In order to build and use Valentina, the PATH environment variable 
needs to be extended:

 PATH               - to locate qmake, moc and other Qt tools
This is done by adding c:\Qt\%VERSION%\bin to the PATH variable.

For newer versions of Windows, PATH can be extended through the 
Control Panel|System|Advanced|Environment variables menu.

You may also need to ensure that the locations of your compiler and 
other build tools are listed in the PATH variable. This will depend on 
your choice of software development environment.

Unix systems:
Default prefix for command "make install" is /usr. For using another 
prefix build with qmake command:

qmake PREFIX=/usr/local PREFIX_LIB=/usr/lib/i386-linux-gnu Valentina.pro -r CONFIG+=noDebugSymbols CONFIG+=no_ccache

where /usr/local is a new prefix for installation binary files and /usr/lib/i386-linux-gnu is new prefix for install libraries.

More about creation installers read in articles:
* How create Windows installer - https://bitbucket.org/dismine/valentina/wiki/developers/Creation_Windows_installer
* How create debian package - https://bitbucket.org/dismine/valentina/wiki/developers/Creation_deb_package

LICENSING
==========
Valentina is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Valentina is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See LICENSE_GPL.txt file for further information

Other components released under:
* QMuParser - BSD license
* VPropertyExplorer - LGPLv2.1 license
