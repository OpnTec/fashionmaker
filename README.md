# Pattern making program

* Author [**Roman Telezhynskyi**](dismine@gmail.com) 
* Copyright © 2013-2019 Valentina project  
* Valentina Web page: https://valentinaproject.bitbucket.io/
* Wiki: https://wiki.valentinaproject.org    
* Valentina main repository: https://bitbucket.org/dismine/valentina/overview    
* Forum http://valentinaproject.forumotion.me/    
* PPA for Ubuntu (develop): https://launchpad.net/~dismine/+archive/valentina-dev    
* Help translate https://www.transifex.com/organization/valentina    

___

## Valentina

**Valentina** is a cross-platform patternmaking program which allows designers 
to create and model patterns of clothing. This software allows pattern 
creation, using either standard sizing tables or an individual’s set of 
measurements. It blends new technologies with traditional methods to create 
a unique pattern making tool. 

## Compiling Valentina

**Prerequisites:**
   
   * Qt 5.4.0 or later (On Unix development packages needed)   
   * Mercurial (only for working with repository)   
   * On **Unix**:     
     - g++ 4.8 or clang 3.4
     - xpdf package (tool pdftops).    
   * On **Windows**:   
     - MinGW  
     - Xpdf is an open source viewer for Portable Document Format (PDF) 
     files. Website http://www.foolabs.com/xpdf/. Put tool pdftops.exe 
     in the same directory with Valentina's binary file.

The installed toolchains have to match the one Qt was compiled with.

You can build Valentina with:

    cd $SOURCE_DIRECTORY\build
    qmake ..\Valentina.pro -r CONFIG+=noDebugSymbols CONFIG+=no_ccache
    make (or mingw32-make or nmake or jom, depending on your platform)

> [!NOTE]
> On some platforms e.g. Fedora, all Qt-tools come as qmake-qt5 etc. In this case you should install qtchooser. This wrapper resolves the Qt version with respect to QT_SELECT. So run: 

    export QT_SELECT=5

    prior to qmake. You will not come along with just using qmake-qt5
    here.


> [!Note]
>In order to build and use Valentina, the PATH environment variable needs to be extended:

**PATH**: To locate qmake, moc and other Qt tools. This is done by adding `C:\Qt\%VERSION%\bin` to the *PATH* variable.

For newer versions of Windows, PATH can be extended through: 
`Control Panel -> System -> Advanced -> Environment variables menu.`

You may also need to ensure that the locations of your compiler and 
other build tools are listed in the PATH variable. This will depend on 
your choice of software development environment.

**Unix systems:**
Default prefix for command `make install` is `/usr`. For using another 
prefix build with qmake command:

`qmake PREFIX=/usr/local PREFIX_LIB=/usr/lib/i386-linux-gnu Valentina.pro -r CONFIG+=noDebugSymbols CONFIG+=no_ccache`

where `/usr/local` is a new prefix for installation binary files and `/usr/lib/i386-linux-gnu` is new prefix for install libraries.

___

## LICENSING

**Valentina** is free software. You can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Valentina is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See [LICENSE_GPL.txt](./LICENSE_GPL.txt) file for further information

*Other components released under:*
* QMuParser - BSD license
* VPropertyExplorer - LGPLv2.1 license
