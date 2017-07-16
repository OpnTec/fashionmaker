/******************************************************************************
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include "dxiface.h"
#include "libdxfrw/libdxfrw.h"
#include "../vmisc/vabstractapplication.h"

#include <iostream>
#include <algorithm>
#include <QDateTime>
#include <QFont>
#include <QLocale>

dx_iface::dx_iface(const std::string &file, DRW::Version v, VarMeasurement varMeasurement, VarInsunits varInsunits)
    : dxfW(new dxfRW(file.c_str())),
      cData(),
      version(v)
{
    InitHeader(varMeasurement, varInsunits);
    InitDefLayers();
    InitTextstyles();
    InitAppId();
}

dx_iface::~dx_iface()
{
    delete dxfW;
}

bool dx_iface::fileExport(bool binary)
{
    bool success = dxfW->write(this, version, binary);
    return success;
}

void dx_iface::writeEntity(DRW_Entity* e){
    switch (e->eType) {
        case DRW::POINT:
        dxfW->writePoint(static_cast<DRW_Point*>(e));
        break;
    case DRW::LINE:
        dxfW->writeLine(static_cast<DRW_Line*>(e));
        break;
    case DRW::CIRCLE:
        dxfW->writeCircle(static_cast<DRW_Circle*>(e));
        break;
    case DRW::ARC:
        dxfW->writeArc(static_cast<DRW_Arc*>(e));
        break;
    case DRW::SOLID:
        dxfW->writeSolid(static_cast<DRW_Solid*>(e));
        break;
    case DRW::ELLIPSE:
        dxfW->writeEllipse(static_cast<DRW_Ellipse*>(e));
        break;
    case DRW::LWPOLYLINE:
        dxfW->writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
    case DRW::POLYLINE:
        dxfW->writePolyline(static_cast<DRW_Polyline*>(e));
        break;
    case DRW::SPLINE:
        dxfW->writeSpline(static_cast<DRW_Spline*>(e));
        break;
//    case RS2::EntitySplinePoints:
//        writeSplinePoints(static_cast<DRW_Point*>(e));
//        break;
//    case RS2::EntityVertex:
//        break;
    case DRW::INSERT:
        dxfW->writeInsert(static_cast<DRW_Insert*>(e));
        break;
    case DRW::MTEXT:
        dxfW->writeMText(static_cast<DRW_MText*>(e));
        break;
    case DRW::TEXT:
        dxfW->writeText(static_cast<DRW_Text*>(e));
        break;
    case DRW::DIMLINEAR:
    case DRW::DIMALIGNED:
    case DRW::DIMANGULAR:
    case DRW::DIMANGULAR3P:
    case DRW::DIMRADIAL:
    case DRW::DIMDIAMETRIC:
    case DRW::DIMORDINATE:
        dxfW->writeDimension(static_cast<DRW_Dimension*>(e));
        break;
    case DRW::LEADER:
        dxfW->writeLeader(static_cast<DRW_Leader*>(e));
        break;
    case DRW::HATCH:
        dxfW->writeHatch(static_cast<DRW_Hatch*>(e));
        break;
    case DRW::IMAGE:
        dxfW->writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
        break;
    default:
        break;
    }
}

void dx_iface::writeHeader(DRW_Header &data){
    //complete copy of header vars:
    data = cData.headerC;
    //or copy one by one:
    //        for (std::map<std::string,DRW_Variant*>::iterator it=cData->headerC.vars.begin(); it != cData->headerC.vars.end(); ++it)
    //            data.vars[it->first] = new DRW_Variant( *(it->second) );
}

void dx_iface::writeBlocks(){
    //write each block
    for (std::list<dx_ifaceBlock*>::iterator it=cData.blocks.begin(); it != cData.blocks.end(); ++it){
        dx_ifaceBlock* bk = *it;
        dxfW->writeBlock(bk);
        //and write each entity in block
        for (std::list<DRW_Entity*>::const_iterator it=bk->ent.begin(); it!=bk->ent.end(); ++it)
            writeEntity(*it);
    }
}

void dx_iface::writeBlockRecords(){
    for (std::list<dx_ifaceBlock*>::iterator it=cData.blocks.begin(); it != cData.blocks.end(); ++it)
        dxfW->writeBlockRecord((*it)->name);
}

void dx_iface::writeEntities(){
    for (std::list<DRW_Entity*>::const_iterator it=cData.mBlock->ent.begin(); it!=cData.mBlock->ent.end(); ++it)
        writeEntity(*it);
}

void dx_iface::writeLTypes(){
    for (std::list<DRW_LType>::iterator it=cData.lineTypes.begin(); it != cData.lineTypes.end(); ++it)
        dxfW->writeLineType(&(*it));
}

void dx_iface::writeLayers(){
    for (std::list<DRW_Layer>::iterator it=cData.layers.begin(); it != cData.layers.end(); ++it)
        dxfW->writeLayer(&(*it));
}

void dx_iface::writeTextstyles(){
    for (std::list<DRW_Textstyle>::iterator it=cData.textStyles.begin(); it != cData.textStyles.end(); ++it)
        dxfW->writeTextstyle(&(*it));
}

void dx_iface::writeVports(){
    for (std::list<DRW_Vport>::iterator it=cData.VPorts.begin(); it != cData.VPorts.end(); ++it)
        dxfW->writeVport(&(*it));
}

void dx_iface::writeDimstyles(){
    for (std::list<DRW_Dimstyle>::iterator it=cData.dimStyles.begin(); it != cData.dimStyles.end(); ++it)
        dxfW->writeDimstyle(&(*it));
}

void dx_iface::writeAppId(){
    for (std::list<DRW_AppId>::iterator it=cData.appIds.begin(); it != cData.appIds.end(); ++it)
        dxfW->writeAppId(&(*it));
}

void dx_iface::InitHeader(VarMeasurement varMeasurement, VarInsunits varInsunits)
{
    cData.headerC.addComment("Valentina DXF File");

    // 1 = Clockwise angles, 0 = Counterclockwise
    cData.headerC.addInt("$ANGDIR", 0, 70);// Qt use counterclockwise

    // Sets drawing units: 0 = English; 1 = Metric
    cData.headerC.addInt("$MEASUREMENT", static_cast<int>(varMeasurement), 70);
    cData.headerC.addInt("$INSUNITS", static_cast<int>(varInsunits), 70);

    QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd.HHmmsszzz");
    dateTime.chop(1);// we need hundredths of a second
    cData.headerC.addStr("$TDCREATE", dateTime.toStdString(), 40);

    if (version >= DRW::AC1021)
    { // Full support Unicode
        cData.headerC.addStr("$DWGCODEPAGE", "UTF-8", 3);
    }
    else
    {
        cData.headerC.addStr("$DWGCODEPAGE", LocaleToISO(), 3);
    }
}

void dx_iface::AddQtLTypes()
{
    DRW_LType ltype;
    ltype.name = "DOT";
    ltype.desc = "Dot . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .";
    ltype.size = 2;
    ltype.length = 0.125;
    ltype.path.push_back(0.0);
    ltype.path.push_back(-0.125);
    cData.lineTypes.push_back(ltype);

    ltype.path.clear();
    ltype.name = "DASHED";
    ltype.desc = "Dashed _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _";
    ltype.size = 2;
    ltype.length = 0.375;
    ltype.path.push_back(0.25);
    ltype.path.push_back(-0.125);
    cData.lineTypes.push_back(ltype);

    ltype.path.clear();
    ltype.name = "DASHDOT2";
    ltype.desc = "Dash dot2 _._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.";
    ltype.size = 4;
    ltype.length = 0.5;
    ltype.path.push_back(0.25);
    ltype.path.push_back(-0.125);
    ltype.path.push_back(0.0);
    ltype.path.push_back(-0.125);
    cData.lineTypes.push_back(ltype);

    ltype.path.clear();
    ltype.name = "DIVIDE2";
    ltype.desc = "Divide2 __..__..__..__..__..__..__..__..__..__..__..__..__..__..__..__..";
    ltype.size = 6;
    ltype.length = 0.625;
    ltype.path.push_back(0.25);
    ltype.path.push_back(-0.125);
    ltype.path.push_back(0.0);
    ltype.path.push_back(-0.125);
    ltype.path.push_back(0.0);
    ltype.path.push_back(-0.125);
    cData.lineTypes.push_back(ltype);
}

void dx_iface::AddAAMALayers()
{
    DRW_Layer layer;

    layer.name = "1";// CUT, OUTLINE
    layer.color = DRW::black;
    cData.layers.push_back(layer);

    layer.name = "8";// DRAW, INK
    layer.color = DRW::black;
    cData.layers.push_back(layer);

    layer.name = "7";// GRAINLINE
    layer.color = DRW::black;
    cData.layers.push_back(layer);

//    layer.name = "6";// MIRROR LINES
//    layer.color = DRW::black;
//    cData.layers.push_back(layer);

    layer.name = "11";// INTCUT
    layer.color = DRW::black;
    cData.layers.push_back(layer);

//    layer.name = "13";// DRILL
//    layer.color = DRW::black;
//    cData.layers.push_back(layer);

    layer.name = "4";// NOTCH
    layer.color = DRW::black;
    cData.layers.push_back(layer);

    layer.name = "19";// TEXT
    layer.color = DRW::black;
    cData.layers.push_back(layer);

//    layer.name = "26";// REF
//    layer.color = DRW::black;
//    cData.layers.push_back(layer);
}

void dx_iface::InitDefLayers()
{
    DRW_Layer defLayer;
    defLayer.name = "0";
    defLayer.color = DRW::black;             // default color
    defLayer.lWeight = DRW_LW_Conv::width03; // default width
    cData.layers.push_back(defLayer);
}

void dx_iface::InitTextstyles()
{
    DRW_Textstyle style;
    style.name = "Standard";
    style.lastHeight = 2.5;
    style.font = "txt";

    cData.textStyles.push_back(style);
}

void dx_iface::InitAppId()
{
    DRW_AppId ai;
    ai.name ="Valentina";
    cData.appIds.push_back(ai);
}

void dx_iface::AddEntity(DRW_Entity *e)
{
    cData.mBlock->ent.push_back(e);
}

UTF8STRING dx_iface::AddFont(const QFont &f)
{
    DRW_Textstyle ts;
    ts.name = f.family().toUpper().toStdString();

    // Idea source https://stackoverflow.com/questions/20111522/writing-text-styles-into-dxf-from-a-delphi-application
    if (f.bold())
    {
        ts.name += "_BOLD";
        ts.fontFamily += 0x2000000;
    }

    if (f.italic())
    {
        ts.name += "_ITALIC";
        ts.fontFamily += 0x1000000;
    }

    for (auto it = cData.textStyles.begin() ; it !=cData.textStyles.end() ; ++it)
    {
        if ((*it).name == ts.name)
        {
            return ts.name;
        }
    }

    ts.font = f.family().toStdString();

    cData.textStyles.push_back(ts);

    return ts.name;
}

void dx_iface::AddBlock(dx_ifaceBlock *block)
{
    cData.blocks.push_back(block);
}

std::string dx_iface::LocaleToISO()
{
    QMap <std::string, std::string> locMap;
    locMap["croatian"] = "ISO8859-2";
    locMap["cs"] = "ISO8859-2";
    locMap["cs_CS"] = "ISO8859-2";
    locMap["cs_CZ"] = "ISO8859-2";
    locMap["cz"] = "ISO8859-2";
    locMap["cz_CZ"] = "ISO8859-2";
    locMap["czech"] = "ISO8859-2";
    locMap["hr"] = "ISO8859-2";
    locMap["hr_HR"] = "ISO8859-2";
    locMap["hu"] = "ISO8859-2";
    locMap["hu_HU"] = "ISO8859-2";
    locMap["hungarian"] = "ISO8859-2";
    locMap["pl"] = "ISO8859-2";
    locMap["pl_PL"] = "ISO8859-2";
    locMap["polish"] = "ISO8859-2";
    locMap["ro"] = "ISO8859-2";
    locMap["ro_RO"] = "ISO8859-2";
    locMap["rumanian"] = "ISO8859-2";
    locMap["serbocroatian"] = "ISO8859-2";
    locMap["sh"] = "ISO8859-2";
    locMap["sh_SP"] = "ISO8859-2";
    locMap["sh_YU"] = "ISO8859-2";
    locMap["sk"] = "ISO8859-2";
    locMap["sk_SK"] = "ISO8859-2";
    locMap["sl"] = "ISO8859-2";
    locMap["sl_CS"] = "ISO8859-2";
    locMap["sl_SI"] = "ISO8859-2";
    locMap["slovak"] = "ISO8859-2";
    locMap["slovene"] = "ISO8859-2";
    locMap["sr_SP"] = "ISO8859-2";

    locMap["eo"] = "ISO8859-3";

    locMap["ee"] = "ISO8859-4";
    locMap["ee_EE"] = "ISO8859-4";

    locMap["mk"] = "ISO8859-5";
    locMap["mk_MK"] = "ISO8859-5";
    locMap["sp"] = "ISO8859-5";
    locMap["sp_YU"] = "ISO8859-5";

    locMap["ar_AA"] = "ISO8859-6";
    locMap["ar_SA"] = "ISO8859-6";
    locMap["arabic"] = "ISO8859-6";

    locMap["el"] = "ISO8859-7";
    locMap["el_GR"] = "ISO8859-7";
    locMap["greek"] = "ISO8859-7";

    locMap["hebrew"] = "ISO8859-8";
    locMap["he"] = "ISO8859-8";
    locMap["he_IL"] = "ISO8859-8";
    locMap["iw"] = "ISO8859-8";
    locMap["iw_IL"] = "ISO8859-8";

    locMap["tr"] = "ISO8859-9";
    locMap["tr_TR"] = "ISO8859-9";
    locMap["turkish"] = "ISO8859-9";

    locMap["lt"] = "ISO8859-13";
    locMap["lt_LT"] = "ISO8859-13";
    locMap["lv"] = "ISO8859-13";
    locMap["lv_LV"] = "ISO8859-13";

    locMap["et"] = "ISO8859-15";
    locMap["et_EE"] = "ISO8859-15";
    locMap["br_FR"] = "ISO8859-15";
    locMap["ca_ES"] = "ISO8859-15";
    locMap["de"] = "ISO8859-15";
    locMap["de_AT"] = "ISO8859-15";
    locMap["de_BE"] = "ISO8859-15";
    locMap["de_DE"] = "ISO8859-15";
    locMap["de_LU"] = "ISO8859-15";
    locMap["en_IE"] = "ISO8859-15";
    locMap["es"] = "ISO8859-15";
    locMap["es_ES"] = "ISO8859-15";
    locMap["eu_ES"] = "ISO8859-15";
    locMap["fi"] = "ISO8859-15";
    locMap["fi_FI"] = "ISO8859-15";
    locMap["finnish"] = "ISO8859-15";
    locMap["fr"] = "ISO8859-15";
    locMap["fr_FR"] = "ISO8859-15";
    locMap["fr_BE"] = "ISO8859-15";
    locMap["fr_LU"] = "ISO8859-15";
    locMap["french"] = "ISO8859-15";
    locMap["ga_IE"] = "ISO8859-15";
    locMap["gl_ES"] = "ISO8859-15";
    locMap["it"] = "ISO8859-15";
    locMap["it_IT"] = "ISO8859-15";
    locMap["oc_FR"] = "ISO8859-15";
    locMap["nl"] = "ISO8859-15";
    locMap["nl_BE"] = "ISO8859-15";
    locMap["nl_NL"] = "ISO8859-15";
    locMap["pt"] = "ISO8859-15";
    locMap["pt_PT"] = "ISO8859-15";
    locMap["sv_FI"] = "ISO8859-15";
    locMap["wa_BE"] = "ISO8859-15";

    locMap["uk"] = "KOI8-U";
    locMap["uk_UA"] = "KOI8-U";
    locMap["ru_YA"] = "KOI8-U";
    locMap["ukrainian"] = "KOI8-U";
    locMap["ru_RU"] = "KOI8-U";

    locMap["be"] = "KOI8-R";
    locMap["be_BY"] = "KOI8-R";
    locMap["bg"] = "KOI8-R";
    locMap["bg_BG"] = "KOI8-R";
    locMap["bulgarian"] = "KOI8-R";
    locMap["ba_RU"] = "KOI8-R";
    locMap["ky"] = "KOI8-R";
    locMap["ky_KG"] = "KOI8-R";
    locMap["kk"] = "KOI8-R";
    locMap["kk_KZ"] = "KOI8-R";

    QLocale locale(qApp->Settings()->GetLocale());
    return locMap.value(locale.name().toStdString(), "ISO8859-1");
}
