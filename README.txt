Pattern making program
Copyright (C) 2013-2014  Roman Telezhynskyi <dismine@gmail.com>    
Valentina Web page: http://www.valentina-project.org/    
Valentina user manual https://bitbucket.org/dismine/valentina/wiki/manual/Content    
Valentina main repository: https://bitbucket.org/dismine/valentina/overview    
Valentina mail list (forum) https://groups.google.com/forum/#!forum/valentina-project-list    
PPA for Ubuntu 14.04(develop): https://launchpad.net/~dismine/+archive/valentina-dev    
Help translate https://www.transifex.com/organization/valentina    

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

Windows XP SP2 or later
Ubuntu Linux 14.04 (32-bit) or later

Building the sources requires Qt 5.2.1 or later.

Compiling Valentina
====================
Prerequisites:   
   * Qt 5.0.2 or later (On Unix development packages needed)   
   * mercurial   
   * On Unix:   
     - ccache   
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

    cd $SOURCE_DIRECTORY
    qmake -r
    make (or mingw32-make or nmake or jom, depending on your platform)

Note:In order to build and use Valentina, the PATH environment variable 
needs to be extended:

 PATH               - to locate qmake, moc and other Qt tools
This is done by adding c:\Qt\%VERSION%\bin to the PATH variable.

For newer versions of Windows, PATH can be extended through the 
Control Panel|System|Advanced|Environment variables menu.

You may also need to ensure that the locations of your compiler and 
other build tools are listed in the PATH variable. This will depend on 
your choice of software development environment.

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

See LICENSE file for further information