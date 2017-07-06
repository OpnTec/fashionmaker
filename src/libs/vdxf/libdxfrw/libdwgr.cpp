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


#include "libdwgr.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include "intern/drw_dbg.h"
#include "intern/drw_textcodec.h"
#include "intern/dwgreader.h"
#include "intern/dwgreader15.h"
#include "intern/dwgreader18.h"
#include "intern/dwgreader21.h"
#include "intern/dwgreader24.h"
#include "intern/dwgreader27.h"

#define FIRSTHANDLE 48

/*enum sections {
    secUnknown,
    secHeader,
    secTables,
    secBlocks,
    secEntities,
    secObjects
};*/

dwgR::dwgR()
    : version(DRW::UNKNOWNV),
      error(DRW::BAD_NONE),
      fileName(),
      applyExt(false),
      codePage(),
      iface(),
      reader(nullptr)
{
    DRW_DBGSL(DRW_dbg::NONE);
}

dwgR::~dwgR(){
    if (reader != nullptr)
        delete reader;

}

void dwgR::setDebug(DRW::DBG_LEVEL lvl){
    switch (lvl){
    case DRW::DEBUG:
        DRW_DBGSL(DRW_dbg::DEBUG);
        break;
    default:
        DRW_DBGSL(DRW_dbg::NONE);
    }
}

/*reads metadata and loads image preview*/
bool dwgR::getPreview(std::istream &stream){
    bool isOk = open(&stream);
    if (!isOk)
        return false;

    isOk = reader->readMetaData();
    if (isOk) {
        isOk = reader->readPreview();
    } else
        error = DRW::BAD_READ_METADATA;

    if (reader != nullptr) {
        delete reader;
        reader = nullptr;
    }
    return isOk;
}

bool dwgR::read(std::istream &stream, DRW_Interface *interface_, bool ext){
    applyExt = ext;
    iface = interface_;

    bool isOk = open(&stream);
    if (!isOk)
        return false;

    isOk = reader->readMetaData();
    if (isOk) {
        isOk = reader->readFileHeader();
        if (isOk) {
            isOk = processDwg();
        } else
            error = DRW::BAD_READ_FILE_HEADER;
    } else
        error = DRW::BAD_READ_METADATA;

    if (reader != nullptr) {
        delete reader;
        reader = nullptr;
    }

    return isOk;
}

bool dwgR::open(std::istream *stream){
    char line[7];
    stream->read (line, 6);
    line[6]='\0';
    DRW_DBG("dwgR::read 2\n");
    DRW_DBG("dwgR::read line version: ");
    DRW_DBG(line);
    DRW_DBG("\n");

    if (strcmp(line, "AC1006") == 0)
        version = DRW::AC1006;
    else if (strcmp(line, "AC1009") == 0) {
        version = DRW::AC1009;
//        reader = new dwgReader09(&filestr, this);
    }else if (strcmp(line, "AC1012") == 0){
        version = DRW::AC1012;
        reader = new dwgReader15(stream, this);
    } else if (strcmp(line, "AC1014") == 0) {
        version = DRW::AC1014;
        reader = new dwgReader15(stream, this);
    } else if (strcmp(line, "AC1015") == 0) {
        version = DRW::AC1015;
        reader = new dwgReader15(stream, this);
    } else if (strcmp(line, "AC1018") == 0){
        version = DRW::AC1018;
        reader = new dwgReader18(stream, this);
    } else if (strcmp(line, "AC1021") == 0) {
        version = DRW::AC1021;
        reader = new dwgReader21(stream, this);
    } else if (strcmp(line, "AC1024") == 0) {
        version = DRW::AC1024;
        reader = new dwgReader24(stream, this);
    } else if (strcmp(line, "AC1027") == 0) {
        version = DRW::AC1027;
        reader = new dwgReader27(stream, this);
    } else
        version = DRW::UNKNOWNV;

    if (reader == NULL) {
        error = DRW::BAD_VERSION;
        return false;
    }
    return true;
}

/********* Reader Process *********/

bool dwgR::processDwg() {
    DRW_DBG("dwgR::processDwg() start processing dwg\n");
    bool ret;
    bool ret2;
    DRW_Header hdr;
    ret = reader->readDwgHeader(hdr);
    if (!ret) {
        error = DRW::BAD_READ_HEADER;
    }

    ret2 = reader->readDwgClasses();
    if (ret && !ret2) {
        error = DRW::BAD_READ_CLASSES;
        ret = ret2;
    }

    ret2 = reader->readDwgHandles();
    if (ret && !ret2) {
        error = DRW::BAD_READ_HANDLES;
        ret = ret2;
    }

    ret2 = reader->readDwgTables(hdr);
    if (ret && !ret2) {
        error = DRW::BAD_READ_TABLES;
        ret = ret2;
    }

    iface->addHeader(&hdr);

    for (std::map<duint32, DRW_LType*>::iterator it=reader->ltypemap.begin(); it!=reader->ltypemap.end(); ++it) {
        DRW_LType *lt = it->second;
        iface->addLType(const_cast<DRW_LType&>(*lt) );
    }
    for (std::map<duint32, DRW_Layer*>::iterator it=reader->layermap.begin(); it!=reader->layermap.end(); ++it) {
        DRW_Layer *ly = it->second;
        iface->addLayer(const_cast<DRW_Layer&>(*ly));
    }

    for (std::map<duint32, DRW_Textstyle*>::iterator it=reader->stylemap.begin(); it!=reader->stylemap.end(); ++it) {
        DRW_Textstyle *ly = it->second;
        iface->addTextStyle(const_cast<DRW_Textstyle&>(*ly));
    }

    for (std::map<duint32, DRW_Dimstyle*>::iterator it=reader->dimstylemap.begin(); it!=reader->dimstylemap.end(); ++it) {
        DRW_Dimstyle *ly = it->second;
        iface->addDimStyle(const_cast<DRW_Dimstyle&>(*ly));
    }

    for (std::map<duint32, DRW_Vport*>::iterator it=reader->vportmap.begin(); it!=reader->vportmap.end(); ++it) {
        DRW_Vport *ly = it->second;
        iface->addVport(const_cast<DRW_Vport&>(*ly));
    }

    for (std::map<duint32, DRW_AppId*>::iterator it=reader->appIdmap.begin(); it!=reader->appIdmap.end(); ++it) {
        DRW_AppId *ly = it->second;
        iface->addAppId(const_cast<DRW_AppId&>(*ly));
    }

    ret2 = reader->readDwgBlocks(*iface);
    if (ret && !ret2) {
        error = DRW::BAD_READ_BLOCKS;
        ret = ret2;
    }

    ret2 = reader->readDwgEntities(*iface);
    if (ret && !ret2) {
        error = DRW::BAD_READ_ENTITIES;
        ret = ret2;
    }

    ret2 = reader->readDwgObjects(*iface);
    if (ret && !ret2) {
        error = DRW::BAD_READ_OBJECTS;
        ret = ret2;
    }

    return ret;
}
