/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
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
#include "libdxfrw/libdwgr.h"
#include "libdxfrw/libdxfrw.h"

#include <iostream>
#include <algorithm>
#include <QDateTime>

dx_iface::dx_iface(const std::string &file, VarMeasurement varMeasurement, VarInsunits varInsunits)
    : dxfW(new dxfRW(file.c_str())),
      cData()
{
    InitHeader(varMeasurement, varInsunits);
    InitLTypes();
    InitLayers();
    InitTextstyles();
}

dx_iface::~dx_iface()
{
    delete dxfW;
}

bool dx_iface::fileExport(DRW::Version v, bool binary)
{
    bool success = dxfW->write(this, v, binary);
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
}

void dx_iface::InitLTypes()
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

void dx_iface::InitLayers()
{
    DRW_Layer defLayer;
    defLayer.name = "0";
    defLayer.color = DRW::black;             // default color
    defLayer.lWeight = DRW_LW_Conv::width17; // default width
    defLayer.lineType = "CONTINUOUS";        // default line style
    cData.layers.push_back(defLayer);

//    DRW_Layer mLayer;
//    mLayer.name = "mainlayer";
//    mLayer.color = DRW::black;      // default color
//    mLayer.lWeight = 100;           // default width
//    mLayer.lineType = "CONTINUOUS"; // default line style
//    cData.lineTypes.push_back(mLayer);
}

void dx_iface::InitTextstyles()
{
    DRW_Textstyle style;
    style.name = "Standard";
    style.lastHeight = 2.5;
    style.font = "txt";

    cData.textStyles.push_back(style);
}

void dx_iface::AddEntity(DRW_Entity *e)
{
    cData.mBlock->ent.push_back(e);
}
