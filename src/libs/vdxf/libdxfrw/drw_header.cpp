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

#include "drw_header.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"
#include "intern/drw_dbg.h"

DRW_Header::DRW_Header()
    : vars(),
      comments(),
      name(),
      curr(),
      version(DRW::AC1021),
      linetypeCtrl(0),
      layerCtrl(0),
      styleCtrl(0),
      dimstyleCtrl(0),
      appidCtrl(0),
      blockCtrl(0),
      viewCtrl(0),
      ucsCtrl(0),
      vportCtrl(0),
      vpEntHeaderCtrl(0)
{}

void DRW_Header::addComment(const std::string &c){
    if (!comments.empty())
        comments += '\n';
    comments += c;
}

void DRW_Header::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 9:
        curr = new DRW_Variant();
        name = reader->getString();
        if (version < DRW::AC1015 && name == "$DIMUNIT")
            name="$DIMLUNIT";
        vars[name]=curr;
        break;
    case 1:
        curr->addString(reader->getUtf8String());
        if (name =="$ACADVER") {
            reader->setVersion(curr->content.s, true);
            version = reader->getVersion();
        }
        curr->code = code;
        break;
    case 2:
    case 6:
    case 7:
    case 8:
    case 390:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    case 3:
        curr->addString(reader->getUtf8String());
        if (name =="$DWGCODEPAGE") {
            reader->setCodePage(curr->content.s);
            curr->addString(reader->getCodePage());
        }
        curr->code = code;
        break;
    case 10:
        curr->addCoord();
        curr->setCoordX(reader->getDouble());
        curr->code = code;
        break;
    case 20:
        curr->setCoordY(reader->getDouble());
        break;
    case 30:
        curr->setCoordZ(reader->getDouble());
        curr->code = code;
        break;
    case 40:
    case 50:
        curr->addDouble(reader->getDouble());
        curr->code = code;
        break;
    case 62:
    case 70:
    case 280:
    case 290:
    case 370:
    case 380:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    default:
        break;
    }
}

void DRW_Header::write(dxfWriter *writer, DRW::Version ver){
/*RLZ: TODO complete all vars to AC1024*/
    double varDouble;
    int varInt;
    std::string varStr;
    DRW_Coord varCoord;
    writer->writeString(2, "HEADER");
    writer->writeString(9, "$ACADVER");
    switch (ver) {
    case DRW::AC1006: //unsupported version acad 10
    case DRW::AC1009: //acad 11 & 12
        varStr = "AC1009";
        break;
    case DRW::AC1012: //unsupported version acad 13
    case DRW::AC1014: //acad 14
        varStr = "AC1014";
        break;
    case DRW::AC1015: //acad 2000
        varStr = "AC1015";
        break;
    case DRW::AC1018: //acad 2004
        varStr = "AC1018";
        break;
    case DRW::AC1024: //acad 2010
        varStr = "AC1024";
        break;
    case DRW::AC1027: //acad 2013
        varStr = "AC1027";
        break;
    case DRW::AC1021: //acad 2007
    default: //acad 2007 default version
        varStr = "AC1021";
        break;
    }
    writer->writeString(1, varStr);
    writer->setVersion(&varStr, true);

    getStr("$ACADVER", &varStr);
    getStr("$ACADMAINTVER", &varStr);

    if (!getStr("$DWGCODEPAGE", &varStr)) {
        varStr = "ANSI_1252";
    }
    writer->writeString(9, "$DWGCODEPAGE");
    writer->setCodePage(&varStr);
    writer->writeString(3, writer->getCodePage() );
    writer->writeString(9, "$INSBASE");
    if (getCoord("$INSBASE", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$EXTMIN");
    if (getCoord("$EXTMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 1.0000000000000000E+020);
        writer->writeDouble(20, 1.0000000000000000E+020);
        writer->writeDouble(30, 1.0000000000000000E+020);
    }
    writer->writeString(9, "$EXTMAX");
    if (getCoord("$EXTMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, -1.0000000000000000E+020);
        writer->writeDouble(20, -1.0000000000000000E+020);
        writer->writeDouble(30, -1.0000000000000000E+020);
    }
    writer->writeString(9, "$LIMMIN");
    if (getCoord("$LIMMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
    }
    writer->writeString(9, "$LIMMAX");
    if (getCoord("$LIMMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 420.0);
        writer->writeDouble(20, 297.0);
    }
    writer->writeString(9, "$ORTHOMODE");
    if (getInt("$ORTHOMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$REGENMODE");
    if (getInt("$REGENMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$FILLMODE");
    if (getInt("$FILLMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$QTEXTMODE");
    if (getInt("$QTEXTMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$MIRRTEXT");
    if (getInt("$MIRRTEXT", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    if (ver == DRW::AC1009){
        writer->writeString(9, "$DRAGMODE");
        if (getInt("$DRAGMODE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 2);
    }
    writer->writeString(9, "$LTSCALE");
    if (getDouble("$LTSCALE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    if (ver == DRW::AC1009){
        writer->writeString(9, "$OSMODE");
        if (getInt("$OSMODE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
    }
    writer->writeString(9, "$ATTMODE");
    if (getInt("$ATTMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$TEXTSIZE");
    if (getDouble("$TEXTSIZE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$TRACEWID");
    if (getDouble("$TRACEWID", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 15.68);
    writer->writeString(9, "$TEXTSTYLE");
    if (getStr("$TEXTSTYLE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(7, varStr);
        else
            writer->writeUtf8String(7, varStr);
    else
        writer->writeString(7, "STANDARD");
    writer->writeString(9, "$CLAYER");
    if (getStr("$CLAYER", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(8, varStr);
        else
            writer->writeUtf8String(8, varStr);
    else
        writer->writeString(8, "0");
    writer->writeString(9, "$CELTYPE");
    if (getStr("$CELTYPE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(6, varStr);
        else
            writer->writeUtf8String(6, varStr);
    else
        writer->writeString(6, "BYLAYER");
    writer->writeString(9, "$CECOLOR");
    if (getInt("$CECOLOR", &varInt))
        writer->writeInt16(62, varInt);
    else
        writer->writeInt16(62, 256);
    if (ver > DRW::AC1009){
        writer->writeString(9, "$CELTSCALE");
        if (getDouble("$CELTSCALE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 1.0);
        writer->writeString(9, "$DISPSILH");
        if (getInt("$DISPSILH", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
    }

    writer->writeString(9, "$DIMSCALE");
    if (getDouble("$DIMSCALE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$DIMASZ");
    if (getDouble("$DIMASZ", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$DIMEXO");
    if (getDouble("$DIMEXO", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.625);
    writer->writeString(9, "$DIMDLI");
    if (getDouble("$DIMDLI", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 3.75);
    writer->writeString(9, "$DIMRND");
    if (getDouble("$DIMRND", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMDLE");
    if (getDouble("$DIMDLE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMEXE");
    if (getDouble("$DIMEXE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.25);
    writer->writeString(9, "$DIMTP");
    if (getDouble("$DIMTP", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMTM");
    if (getDouble("$DIMTM", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMTXT");
    if (getDouble("$DIMTXT", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$DIMCEN");
    if (getDouble("$DIMCEN", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$DIMTSZ");
    if (getDouble("$DIMTSZ", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMTOL");
    if (getInt("$DIMTOL", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMLIM");
    if (getInt("$DIMLIM", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMTIH");
    if (getInt("$DIMTIH", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMTOH");
    if (getInt("$DIMTOH", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMSE1");
    if (getInt("$DIMSE1", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMSE2");
    if (getInt("$DIMSE2", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMTAD");
    if (getInt("$DIMTAD", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$DIMZIN");
    if (getInt("$DIMZIN", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 8);
    writer->writeString(9, "$DIMBLK");
    if (getStr("$DIMBLK", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, "");
    writer->writeString(9, "$DIMASO");
    if (getInt("$DIMASO", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$DIMSHO");
    if (getInt("$DIMSHO", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$DIMPOST");
    if (getStr("$DIMPOST", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, "");
    writer->writeString(9, "$DIMAPOST");
    if (getStr("$DIMAPOST", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, "");
    writer->writeString(9, "$DIMALT");
    if (getInt("$DIMALT", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMALTD");
    if (getInt("$DIMALTD", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 3);
    writer->writeString(9, "$DIMALTF");
    if (getDouble("$DIMALTF", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.03937);
    writer->writeString(9, "$DIMLFAC");
    if (getDouble("$DIMLFAC", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$DIMTOFL");
    if (getInt("$DIMTOFL", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$DIMTVP");
    if (getDouble("$DIMTVP", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$DIMTIX");
    if (getInt("$DIMTIX", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMSOXD");
    if (getInt("$DIMSOXD", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMSAH");
    if (getInt("$DIMSAH", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMBLK1");
    if (getStr("$DIMBLK1", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, "");
    writer->writeString(9, "$DIMBLK2");
    if (getStr("$DIMBLK2", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, "");
    writer->writeString(9, "$DIMSTYLE");
    if (getStr("$DIMSTYLE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(2, varStr);
        else
            writer->writeUtf8String(2, varStr);
    else
        writer->writeString(2, "STANDARD");
    writer->writeString(9, "$DIMCLRD");
    if (getInt("$DIMCLRD", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMCLRE");
    if (getInt("$DIMCLRE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMCLRT");
    if (getInt("$DIMCLRT", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$DIMTFAC");
    if (getDouble("$DIMTFAC", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$DIMGAP");
    if (getDouble("$DIMGAP", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.625);
    //post r12 dim vars
    if (ver > DRW::AC1009) {
        writer->writeString(9, "$DIMJUST");
        if (getInt("$DIMJUST", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMSD1");
        if (getInt("$DIMSD1", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMSD2");
        if (getInt("$DIMSD2", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMTOLJ");
        if (getInt("$DIMTOLJ", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMTZIN");
        if (getInt("$DIMTZIN", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 8);
        writer->writeString(9, "$DIMALTZ");
        if (getInt("$DIMALTZ", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMALTTZ");
        if (getInt("$DIMALTTZ", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMUPT");
        if (getInt("$DIMUPT", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMDEC");
        if (getInt("$DIMDEC", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 2);
        writer->writeString(9, "$DIMTDEC");
        if (getInt("$DIMTDEC", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 2);
        writer->writeString(9, "$DIMALTU");
        if (getInt("$DIMALTU", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 2);
        writer->writeString(9, "$DIMALTTD");
        if (getInt("$DIMALTTD", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 3);
        writer->writeString(9, "$DIMTXSTY");
        if (getStr("$DIMTXSTY", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(7, varStr);
            else
                writer->writeUtf8String(7, varStr);
        else
            writer->writeString(7, "STANDARD");
        writer->writeString(9, "$DIMAUNIT");
        if (getInt("$DIMAUNIT", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMADEC");
        if (getInt("$DIMADEC", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMALTRND");
        if (getDouble("$DIMALTRND", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$DIMAZIN");
        if (getInt("$DIMAZIN", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMDSEP");
        if (getInt("$DIMDSEP", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 44);
        writer->writeString(9, "$DIMATFIT");
        if (getInt("$DIMATFIT", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 3);
        writer->writeString(9, "$DIMFRAC");
        if (getInt("$DIMFRAC", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMLDRBLK");
        if (getStr("$DIMLDRBLK", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(1, varStr);
            else
                writer->writeUtf8String(1, varStr);
        else
            writer->writeString(1, "STANDARD");
    //verify if exist "$DIMLUNIT" or obsolete "$DIMUNIT" (pre v2000)
        if ( !getInt("$DIMLUNIT", &varInt) ){
            if (!getInt("$DIMUNIT", &varInt))
                varInt = 2;
        }
        //verify valid values from 1 to 6
        if (varInt<1 || varInt>6)
            varInt = 2;
        if (ver > DRW::AC1014) {
            writer->writeString(9, "$DIMLUNIT");
            writer->writeInt16(70, varInt);
        } else {
            writer->writeString(9, "$DIMUNIT");
            writer->writeInt16(70, varInt);
        }
        writer->writeString(9, "$DIMLWD");
        if (getInt("$DIMLWD", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, -2);
        writer->writeString(9, "$DIMLWE");
        if (getInt("$DIMLWE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, -2);
        writer->writeString(9, "$DIMTMOVE");
        if (getInt("$DIMTMOVE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);

        if (ver > DRW::AC1018) {// and post v2004 dim vars
            writer->writeString(9, "$DIMFXL");
            if (getDouble("$DIMFXL", &varDouble))
                writer->writeDouble(40, varDouble);
            else
                writer->writeDouble(40, 1.0);
            writer->writeString(9, "$DIMFXLON");
            if (getInt("$DIMFXLON", &varInt))
                writer->writeInt16(70, varInt);
            else
                writer->writeInt16(70, 0);
            writer->writeString(9, "$DIMJOGANG");
            if (getDouble("$DIMJOGANG", &varDouble))
                writer->writeDouble(40, varDouble);
            else
                writer->writeDouble(40, 0.7854);
            writer->writeString(9, "$DIMTFILL");
            if (getInt("$DIMTFILL", &varInt))
                writer->writeInt16(70, varInt);
            else
                writer->writeInt16(70, 0);
            writer->writeString(9, "$DIMTFILLCLR");
            if (getInt("$DIMTFILLCLR", &varInt))
                writer->writeInt16(70, varInt);
            else
                writer->writeInt16(70, 0);
            writer->writeString(9, "$DIMARCSYM");
            if (getInt("$DIMARCSYM", &varInt))
                writer->writeInt16(70, varInt);
            else
                writer->writeInt16(70, 0);
            writer->writeString(9, "$DIMLTYPE");
            if (getStr("$DIMLTYPE", &varStr))
                if (ver == DRW::AC1009)
                    writer->writeUtf8Caps(6, varStr);
                else
                    writer->writeUtf8String(6, varStr);
            else
                writer->writeString(6, "");
            writer->writeString(9, "$DIMLTEX1");
            if (getStr("$DIMLTEX1", &varStr))
                if (ver == DRW::AC1009)
                    writer->writeUtf8Caps(6, varStr);
                else
                    writer->writeUtf8String(6, varStr);
            else
                writer->writeString(6, "");
            writer->writeString(9, "$DIMLTEX2");
            if (getStr("$DIMLTEX2", &varStr))
                if (ver == DRW::AC1009)
                    writer->writeUtf8Caps(6, varStr);
                else
                    writer->writeUtf8String(6, varStr);
            else
                writer->writeString(6, "");
            if (ver > DRW::AC1021) {// and post v2007 dim vars
                writer->writeString(9, "$DIMTXTDIRECTION");
                if (getInt("$DIMTXTDIRECTION", &varInt))
                    writer->writeInt16(70, varInt);
                else
                    writer->writeInt16(70, 0);
            }
        }// end post v2004 dim vars
    }//end post r12 dim vars

    writer->writeString(9, "$LUNITS");
    if (getInt("$LUNITS", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 2);
    writer->writeString(9, "$LUPREC");
    if (getInt("$LUPREC", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 4);
    writer->writeString(9, "$SKETCHINC");
    if (getDouble("$SKETCHINC", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$FILLETRAD");
    if (getDouble("$FILLETRAD", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$AUNITS");
    if (getInt("$AUNITS", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$AUPREC");
    if (getInt("$AUPREC", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 2);
    writer->writeString(9, "$MENU");
    if (getStr("$MENU", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(1, varStr);
        else
            writer->writeUtf8String(1, varStr);
    else
        writer->writeString(1, ".");
    writer->writeString(9, "$ELEVATION");
    if (getDouble("$ELEVATION", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$PELEVATION");
    if (getDouble("$PELEVATION", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$THICKNESS");
    if (getDouble("$THICKNESS", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$LIMCHECK");
    if (getInt("$LIMCHECK", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    if (ver < DRW::AC1015) {
        writer->writeString(9, "$BLIPMODE");
        if (getInt("$BLIPMODE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
    }
    writer->writeString(9, "$CHAMFERA");
    if (getDouble("$CHAMFERA", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$CHAMFERB");
    if (getDouble("$CHAMFERB", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    if (ver > DRW::AC1009) {
        writer->writeString(9, "$CHAMFERC");
        if (getDouble("$CHAMFERC", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$CHAMFERD");
        if (getDouble("$CHAMFERD", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
    }
    writer->writeString(9, "$SKPOLY");
    if (getInt("$SKPOLY", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 0);
    //rlz: todo, times
    writer->writeString(9, "$USRTIMER");
    if (getInt("$USRTIMER", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$ANGBASE");
    if (getDouble("$ANGBASE", &varDouble))
        writer->writeDouble(50, varDouble);
    else
        writer->writeDouble(50, 0.0);
    writer->writeString(9, "$ANGDIR");
    if (getInt("$ANGDIR", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$PDMODE");
    if (getInt("$PDMODE", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 34);
    writer->writeString(9, "$PDSIZE");
    if (getDouble("$PDSIZE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$PLINEWID");
    if (getDouble("$PLINEWID", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    if (ver < DRW::AC1012) {
        writer->writeString(9, "$COORDS");
        if (getInt("$COORDS", &varInt)) {
            writer->writeInt16(70, varInt);
        } else
            writer->writeInt16(70, 2);
    }
    writer->writeString(9, "$SPLFRAME");
    if (getInt("$SPLFRAME", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$SPLINETYPE");
    if (getInt("$SPLINETYPE", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 2);
    writer->writeString(9, "$SPLINESEGS");
    if (getInt("$SPLINESEGS", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 8);
    if (ver < DRW::AC1012) {
        writer->writeString(9, "$ATTDIA");
        if (getInt("$ATTDIA", &varInt)) {
            writer->writeInt16(70, varInt);
        } else
            writer->writeInt16(70, 1);
        writer->writeString(9, "$ATTREQ");
        if (getInt("$ATTREQ", &varInt)) {
            writer->writeInt16(70, varInt);
        } else
            writer->writeInt16(70, 1);
        writer->writeString(9, "$HANDLING");
        if (getInt("$HANDLING", &varInt)) {
            writer->writeInt16(70, varInt);
        } else
            writer->writeInt16(70, 1);
    }
    writer->writeString(9, "$HANDSEED");
    //RLZ        dxfHex(5, 0xFFFF);
    writer->writeString(5, "20000");
    writer->writeString(9, "$SURFTAB1");
    if (getInt("$SURFTAB1", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 6);
    writer->writeString(9, "$SURFTAB2");
    if (getInt("$SURFTAB2", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 6);
    writer->writeString(9, "$SURFTYPE");
    if (getInt("$SURFTYPE", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 6);
    writer->writeString(9, "$SURFU");
    if (getInt("$SURFU", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 6);
    writer->writeString(9, "$SURFV");
    if (getInt("$SURFV", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 6);
    if (getStr("$TDCREATE", &varStr)) {
        writer->writeString(9, "$TDCREATE");
        writer->writeString(40, varStr);
    }
    if (ver > DRW::AC1009) {
    writer->writeString(9, "$UCSBASE");
    if (getStr("$UCSBASE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(2, varStr);
        else
            writer->writeUtf8String(2, varStr);
    else
        writer->writeString(2, "");
}
    writer->writeString(9, "$UCSNAME");
    if (getStr("$UCSNAME", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(2, varStr);
        else
            writer->writeUtf8String(2, varStr);
    else
        writer->writeString(2, "");
    writer->writeString(9, "$UCSORG");
    if (getCoord("$UCSORG", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$UCSXDIR");
    if (getCoord("$UCSXDIR", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 1.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$UCSYDIR");
    if (getCoord("$UCSYDIR", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 1.0);
        writer->writeDouble(30, 0.0);
    }
    if (ver > DRW::AC1009) { //begin post r12 UCS vars
        writer->writeString(9, "$UCSORTHOREF");
        if (getStr("$UCSORTHOREF", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(2, varStr);
            else
                writer->writeUtf8String(2, varStr);
        else
            writer->writeString(2, "");
        writer->writeString(9, "$UCSORTHOVIEW");
        if (getInt("$UCSORTHOVIEW", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$UCSORGTOP");
        if (getCoord("$UCSORGTOP", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$UCSORGBOTTOM");
        if (getCoord("$UCSORGBOTTOM", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$UCSORGLEFT");
        if (getCoord("$UCSORGLEFT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$UCSORGRIGHT");
        if (getCoord("$UCSORGRIGHT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$UCSORGFRONT");
        if (getCoord("$UCSORGFRONT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$UCSORGBACK");
        if (getCoord("$UCSORGBACK", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSBASE");
        if (getStr("$PUCSBASE", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(2, varStr);
            else
                writer->writeUtf8String(2, varStr);
        else
            writer->writeString(2, "");
    } //end post r12 UCS vars
    writer->writeString(9, "$PUCSNAME");
    if (getStr("$PUCSNAME", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(2, varStr);
        else
            writer->writeUtf8String(2, varStr);
    else
        writer->writeString(2, "");
    writer->writeString(9, "$PUCSORG");
    if (getCoord("$PUCSORG", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$PUCSXDIR");
    if (getCoord("$PUCSXDIR", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 1.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$PUCSYDIR");
    if (getCoord("$PUCSYDIR", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 1.0);
        writer->writeDouble(30, 0.0);
    }
    if (ver > DRW::AC1009) { //begin post r12 PUCS vars
        writer->writeString(9, "$PUCSORTHOREF");
        if (getStr("$PUCSORTHOREF", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(2, varStr);
            else
                writer->writeUtf8String(2, varStr);
        else
            writer->writeString(2, "");
        writer->writeString(9, "$PUCSORTHOVIEW");
        if (getInt("$PUCSORTHOVIEW", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$PUCSORGTOP");
        if (getCoord("$PUCSORGTOP", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSORGBOTTOM");
        if (getCoord("$PUCSORGBOTTOM", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSORGLEFT");
        if (getCoord("$PUCSORGLEFT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSORGRIGHT");
        if (getCoord("$PUCSORGRIGHT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSORGFRONT");
        if (getCoord("$PUCSORGFRONT", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
        writer->writeString(9, "$PUCSORGBACK");
        if (getCoord("$PUCSORGBACK", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
    } //end post r12 PUCS vars

    writer->writeString(9, "$USERI1");
    if (getInt("$USERI1", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$USERI2");
    if (getInt("$USERI2", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$USERI3");
    if (getInt("$USERI3", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$USERI4");
    if (getInt("$USERI4", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$USERI5");
    if (getInt("$USERI5", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$USERR1");
    if (getDouble("$USERR1", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$USERR2");
    if (getDouble("$USERR2", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$USERR3");
    if (getDouble("$USERR3", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$USERR4");
    if (getDouble("$USERR4", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$USERR5");
    if (getDouble("$USERR5", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    writer->writeString(9, "$WORLDVIEW");
    if (getInt("$WORLDVIEW", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$SHADEDGE");
    if (getInt("$SHADEDGE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 3);
    writer->writeString(9, "$SHADEDIF");
    if (getInt("$SHADEDIF", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 70);
    writer->writeString(9, "$TILEMODE");
    if (getInt("$TILEMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$MAXACTVP");
    if (getInt("$MAXACTVP", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 64);
    if (ver > DRW::AC1009) { //begin post r12 PUCS vars
        writer->writeString(9, "$PINSBASE");
        if (getCoord("$PINSBASE", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
    }
    writer->writeString(9, "$PLIMCHECK");
    if (getInt("$PLIMCHECK", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$PEXTMIN");
    if (getCoord("$PEXTMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$PEXTMAX");
    if (getCoord("$PEXTMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }

/* RLZ: moved to active VPORT, but can write in header if present*/
    if (getInt("$GRIDMODE", &varInt)) {
        writer->writeString(9, "$GRIDMODE");
        writer->writeInt16(70, varInt);
    }
    if (getInt("$SNAPSTYLE", &varInt)) {
        writer->writeString(9, "$SNAPSTYLE");
        writer->writeInt16(70, varInt);
    }
    if (getCoord("$GRIDUNIT", &varCoord)) {
        writer->writeString(9, "$GRIDUNIT");
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    }
    if (getCoord("$VIEWCTR", &varCoord)) {
        writer->writeString(9, "$VIEWCTR");
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    }
/* RLZ: moved to active VPORT, but can write in header if present*/

    writer->writeString(9, "$PLIMMIN");
    if (getCoord("$PLIMMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
    }
    writer->writeString(9, "$PLIMMAX");
    if (getCoord("$PLIMMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 297.0);
        writer->writeDouble(20, 210.0);
    }
    writer->writeString(9, "$UNITMODE");
    if (getInt("$UNITMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$VISRETAIN");
    if (getInt("$VISRETAIN", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    writer->writeString(9, "$PLINEGEN");
    if (getInt("$PLINEGEN", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$PSLTSCALE");
    if (getInt("$PSLTSCALE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 1);
    if (ver > DRW::AC1009){//start port r12 vars
        writer->writeString(9, "$TREEDEPTH");
        if (getInt("$TREEDEPTH", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 3020);
        writer->writeString(9, "$CMLSTYLE");
        if (getStr("$CMLSTYLE", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(2, varStr);
            else
                writer->writeUtf8String(2, varStr);
        else
            writer->writeString(2, "Standard");
        writer->writeString(9, "$CMLJUST");
        if (getInt("$CMLJUST", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$CMLSCALE");
        if (getDouble("$CMLSCALE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 20.0);
        writer->writeString(9, "$PROXYGRAPHICS");
        if (getInt("$PROXYGRAPHICS", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 1);
        writer->writeString(9, "$MEASUREMENT");
        if (getInt("$MEASUREMENT", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 1);
        writer->writeString(9, "$CELWEIGHT");
        if (getInt("$CELWEIGHT", &varInt))
            writer->writeInt16(370, varInt);
        else
            writer->writeInt16(370, -1);
        writer->writeString(9, "$ENDCAPS");
        if (getInt("$ENDCAPS", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$JOINSTYLE");
        if (getInt("$JOINSTYLE", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$LWDISPLAY"); //RLZ bool flag, verify in bin version
        if (getInt("$LWDISPLAY", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 0);
        if (ver > DRW::AC1014) {
            writer->writeString(9, "$INSUNITS");
            if (getInt("$INSUNITS", &varInt))
                writer->writeInt16(70, varInt);
            else
                writer->writeInt16(70, 0);
        }
        writer->writeString(9, "$HYPERLINKBASE");
        if (getStr("$HYPERLINKBASE", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(1, varStr);
            else
                writer->writeUtf8String(1, varStr);
        else
            writer->writeString(1, "");
        writer->writeString(9, "$STYLESHEET");
        if (getStr("$STYLESHEET", &varStr))
            if (ver == DRW::AC1009)
                writer->writeUtf8Caps(1, varStr);
            else
                writer->writeUtf8String(1, varStr);
        else
            writer->writeString(1, "");
        writer->writeString(9, "$XEDIT"); //RLZ bool flag, verify in bin version
        if (getInt("$XEDIT", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 1);
        writer->writeString(9, "$CEPSNTYPE");
        if (getInt("$CEPSNTYPE", &varInt))
            writer->writeInt16(380, varInt);
        else
            writer->writeInt16(380, 0);
        writer->writeString(9, "$PSTYLEMODE"); //RLZ bool flag, verify in bin version
        if (getInt("$PSTYLEMODE", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 1);
//RLZ: here $FINGERPRINTGUID and $VERSIONGUID, do not add?
        writer->writeString(9, "$EXTNAMES"); //RLZ bool flag, verify in bin version
        if (getInt("$EXTNAMES", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 1);
        writer->writeString(9, "$PSVPSCALE");
        if (getDouble("$PSVPSCALE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$OLESTARTUP"); //RLZ bool flag, verify in bin version
        if (getInt("$OLESTARTUP", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 0);
    }
    if (ver > DRW::AC1015) {// and post v2004 vars
        writer->writeString(9, "$SORTENTS");
        if (getInt("$SORTENTS", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 127);
        writer->writeString(9, "$INDEXCTL");
        if (getInt("$INDEXCTL", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$HIDETEXT");
        if (getInt("$HIDETEXT", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$XCLIPFRAME"); //RLZ bool flag, verify in bin version
        if (ver > DRW::AC1021) {
            if (getInt("$XCLIPFRAME", &varInt))
                writer->writeInt16(280, varInt);
            else
                writer->writeInt16(280, 0);
        } else {
            if (getInt("$XCLIPFRAME", &varInt))
                writer->writeInt16(290, varInt);
            else
                writer->writeInt16(290, 0);
        }
        writer->writeString(9, "$HALOGAP");
        if (getInt("$HALOGAP", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$OBSCOLOR");
        if (getInt("$OBSCOLOR", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 257);
        writer->writeString(9, "$OBSLTYPE");
        if (getInt("$OBSLTYPE", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$INTERSECTIONDISPLAY");
        if (getInt("$INTERSECTIONDISPLAY", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$INTERSECTIONCOLOR");
        if (getInt("$INTERSECTIONCOLOR", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 257);
        writer->writeString(9, "$DIMASSOC");
        if (getInt("$DIMASSOC", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$PROJECTNAME");
        if (getStr("$PROJECTNAME", &varStr))
            writer->writeUtf8String(1, varStr);
        else
            writer->writeString(1, "");
    }
    if (ver > DRW::AC1018) {// and post v2007 vars
        writer->writeString(9, "$CAMERADISPLAY"); //RLZ bool flag, verify in bin version
        if (getInt("$CAMERADISPLAY", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 0);
        writer->writeString(9, "$LENSLENGTH");
        if (getDouble("$LENSLENGTH", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 50.0);
        writer->writeString(9, "$CAMERAHEIGHT");
        if (getDouble("$CAMERAHEIGTH", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$STEPSPERSEC");
        if (getDouble("$STEPSPERSEC", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 2.0);
        writer->writeString(9, "$STEPSIZE");
        if (getDouble("$STEPSIZE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 50.0);
        writer->writeString(9, "$3DDWFPREC");
        if (getDouble("$3DDWFPREC", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 2.0);
        writer->writeString(9, "$PSOLWIDTH");
        if (getDouble("$PSOLWIDTH", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 5.0);
        writer->writeString(9, "$PSOLHEIGHT");
        if (getDouble("$PSOLHEIGHT", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 80.0);
        writer->writeString(9, "$LOFTANG1");
        if (getDouble("$LOFTANG1", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, M_PI_2);
        writer->writeString(9, "$LOFTANG2");
        if (getDouble("$LOFTANG2", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, M_PI_2);
        writer->writeString(9, "$LOFTMAG1");
        if (getDouble("$LOFTMAG1", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$LOFTMAG2");
        if (getDouble("$LOFTMAG2", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$LOFTPARAM");
        if (getInt("$LOFTPARAM", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 7);
        writer->writeString(9, "$LOFTNORMALS");
        if (getInt("$LOFTNORMALS", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$LATITUDE");
        if (getDouble("$LATITUDE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 1.0);
        writer->writeString(9, "$LONGITUDE");
        if (getDouble("$LONGITUDE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 1.0);
        writer->writeString(9, "$NORTHDIRECTION");
        if (getDouble("$LONGITUDE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
        writer->writeString(9, "$TIMEZONE");
        if (getInt("$TIMEZONE", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, -8000);
        writer->writeString(9, "$LIGHTGLYPHDISPLAY");
        if (getInt("$LIGHTGLYPHDISPLAY", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$TILEMODELIGHTSYNCH");
        if (getInt("$TILEMODELIGHTSYNCH", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
    //$CMATERIAL is a handle
        writer->writeString(9, "$SOLIDHIST");
        if (getInt("$SOLIDHIST", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$SHOWHIST");
        if (getInt("$SHOWHIST", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 1);
        writer->writeString(9, "$DWFFRAME");
        if (getInt("$DWFFRAME", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 2);
        writer->writeString(9, "$DGNFRAME");
        if (getInt("$DGNFRAME", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$REALWORLDSCALE"); //RLZ bool flag, verify in bin version
        if (getInt("$REALWORLDSCALE", &varInt))
            writer->writeInt16(290, varInt);
        else
            writer->writeInt16(290, 1);
        writer->writeString(9, "$INTERFERECOLOR");
        if (getInt("$INTERFERECOLOR", &varInt))
            writer->writeInt16(62, varInt);
        else
            writer->writeInt16(62, 1);
        //$INTERFEREOBJVS is a handle
        //$INTERFEREVPVS is a handle
        writer->writeString(9, "$CSHADOW");
        if (getInt("$CSHADOW", &varInt))
            writer->writeInt16(280, varInt);
        else
            writer->writeInt16(280, 0);
        writer->writeString(9, "$SHADOWPLANELOCATION");
        if (getDouble("$SHADOWPLANELOCATION", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
    }

#ifdef DRW_DBG
    std::map<std::string,DRW_Variant *>::const_iterator it;
    for ( it=vars.begin() ; it != vars.end(); ++it ){
        DRW_DBG((*it).first); DRW_DBG("\n");
    }
#endif
}

void DRW_Header::addDouble(std::string key, double value, int code){
    // cppcheck-suppress publicAllocationError
    curr = new DRW_Variant();
    curr->addDouble( value );
    curr->code = code;
    vars[key] =curr;
}

void DRW_Header::addInt(std::string key, int value, int code){
    // cppcheck-suppress publicAllocationError
    curr = new DRW_Variant();
    curr->addInt( value );
    curr->code = code;
    vars[key] =curr;
}

void DRW_Header::addStr(std::string key, std::string value, int code){
    // cppcheck-suppress publicAllocationError
    curr = new DRW_Variant();
    curr->addString( value );
    curr->code = code;
    vars[key] =curr;
}

void DRW_Header::addCoord(std::string key, DRW_Coord value, int code){
    // cppcheck-suppress publicAllocationError
    curr = new DRW_Variant();
    curr->addCoord( value );
    curr->code = code;
    vars[key] =curr;
}

bool DRW_Header::getDouble(std::string key, double *varDouble){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::DOUBLE) {
            *varDouble = var->content.d;
            result = true;
        }
        delete var;
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getInt(std::string key, int *varInt){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::INTEGER) {
            *varInt = var->content.i;
            result = true;
        }
        delete var;
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getStr(std::string key, std::string *varStr){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::STRING) {
            *varStr = *var->content.s;
            result = true;
        }
        delete var;
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getCoord(std::string key, DRW_Coord *varCoord){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::COORD) {
            *varCoord = *var->content.v;
            result = true;
        }
        delete var;
        vars.erase (it);
    }
    return result;
}
