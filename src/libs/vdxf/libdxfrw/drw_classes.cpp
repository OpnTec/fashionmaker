/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2011-2015 José F. Soriano, rallazz@gmail.com               **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include "drw_classes.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"
#include "intern/drw_dbg.h"

void DRW_Class::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 1:
        recName = reader->getUtf8String();
        break;
    case 2:
        className = reader->getUtf8String();
        break;
    case 3:
        appName = reader->getUtf8String();
        break;
    case 90:
        proxyFlag = reader->getInt32();
        break;
    case 91:
        instanceCount = reader->getInt32();
        break;
    case 280:
        wasaProxyFlag = reader->getInt32();
        break;
    case 281:
        entityFlag = reader->getInt32();
        break;
    default:
        break;
    }
}

void DRW_Class::write(dxfWriter *writer, DRW::Version ver){
    if (ver > DRW::AC1009) {
        writer->writeString(0, "CLASS");
        writer->writeString(1, recName);
        writer->writeString(2, className);
        writer->writeString(3, appName);
        writer->writeInt32(90, proxyFlag);
        if (ver > DRW::AC1015) { //2004+
            writer->writeInt32(91, instanceCount);
        }
        writer->writeInt16(280, wasaProxyFlag);
        writer->writeInt16(281, entityFlag);
    }
}
