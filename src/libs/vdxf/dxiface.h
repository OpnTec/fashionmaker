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

#ifndef DX_IFACE_H
#define DX_IFACE_H

#include "libdxfrw/drw_interface.h"
#include "libdxfrw/libdxfrw.h"
#include "dxfdef.h"

//class to store image data and path from DRW_ImageDef
class dx_ifaceImg : public DRW_Image {
public:
    dx_ifaceImg(){}
    dx_ifaceImg(const DRW_Image& p):DRW_Image(p){}
    ~dx_ifaceImg(){}
    std::string path; //stores the image path
};

//container class to store entites.
class dx_ifaceBlock : public DRW_Block {
public:
    dx_ifaceBlock(){}
    dx_ifaceBlock(const DRW_Block& p):DRW_Block(p){}
    ~dx_ifaceBlock(){
        for (std::list<DRW_Entity*>::const_iterator it=ent.begin(); it!=ent.end(); ++it)
            delete *it;
    }
    std::list<DRW_Entity*>ent; //stores the entities list
};


//container class to store full dwg/dxf data.
class dx_data {
public:
    dx_data(){
        mBlock = new dx_ifaceBlock();
    }
    ~dx_data(){
        //cleanup,
        for (std::list<dx_ifaceBlock*>::const_iterator it=blocks.begin(); it!=blocks.end(); ++it)
            delete *it;
        delete mBlock;
    }

    DRW_Header headerC;                 //stores a copy of the header vars
    std::list<DRW_LType>lineTypes;      //stores a copy of all line types
    std::list<DRW_Layer>layers;         //stores a copy of all layers
    std::list<DRW_Dimstyle>dimStyles;   //stores a copy of all dimension styles
    std::list<DRW_Vport>VPorts;         //stores a copy of all vports
    std::list<DRW_Textstyle>textStyles; //stores a copy of all text styles
    std::list<DRW_AppId>appIds;         //stores a copy of all line types
    std::list<dx_ifaceBlock*>blocks;    //stores a copy of all blocks and the entities in it
    std::list<dx_ifaceImg*>images;      //temporary list to find images for link with DRW_ImageDef. Do not delete it!!

    dx_ifaceBlock* mBlock;              //container to store model entities


};

class dx_iface : public DRW_Interface
{
public:
    dx_iface(const std::string& file, VarMeasurement varMeasurement, VarInsunits varInsunits);
    virtual ~dx_iface();
    bool fileExport(DRW::Version v, bool binary);
    void writeEntity(DRW_Entity* e);

//reimplement virtual DRW_Interface functions

//reader part, do nothing
    //header
    void addHeader(const DRW_Header* data){
        // do nothing
    }

    //tables
    virtual void addLType(const DRW_LType&){
        // do nothing
    }
    virtual void addLayer(const DRW_Layer&){
        // do nothing
    }
    virtual void addDimStyle(const DRW_Dimstyle&){
        // do nothing
    }
    virtual void addVport(const DRW_Vport&){
        // do nothing
    }
    virtual void addTextStyle(const DRW_Textstyle&){
        // do nothing
    }
    virtual void addAppId(const DRW_AppId&){
        // do nothing
    }

    //blocks
    virtual void addBlock(const DRW_Block&){
        // do nothing
    }
    virtual void endBlock(){
        // do nothing
    }

    virtual void setBlock(const int /*handle*/){}//unused

    //entities
    virtual void addPoint(const DRW_Point&){
        // do nothing
    }
    virtual void addLine(const DRW_Line&){
        // do nothing
    }
    virtual void addRay(const DRW_Ray&){
        // do nothing
    }
    virtual void addXline(const DRW_Xline&){
        // do nothing
    }
    virtual void addArc(const DRW_Arc&){
        // do nothing
    }
    virtual void addCircle(const DRW_Circle&){
        // do nothing
    }
    virtual void addEllipse(const DRW_Ellipse&){
        // do nothing
    }
    virtual void addLWPolyline(const DRW_LWPolyline&){
        // do nothing
    }
    virtual void addPolyline(const DRW_Polyline&){
        // do nothing
    }
    virtual void addSpline(const DRW_Spline*){
        // do nothing
    }
    // ¿para que se usa?
    virtual void addKnot(const DRW_Entity&){}

    virtual void addInsert(const DRW_Insert&){
        // do nothing
    }
    virtual void addTrace(const DRW_Trace&){
        // do nothing
    }
    virtual void add3dFace(const DRW_3Dface&){
        // do nothing
    }
    virtual void addSolid(const DRW_Solid&){
        // do nothing
    }
    virtual void addMText(const DRW_MText&){
        // do nothing
    }
    virtual void addText(const DRW_Text&){
        // do nothing
    }
    virtual void addDimAlign(const DRW_DimAligned *){
        // do nothing
    }
    virtual void addDimLinear(const DRW_DimLinear *){
        // do nothing
    }
    virtual void addDimRadial(const DRW_DimRadial *){
        // do nothing
    }
    virtual void addDimDiametric(const DRW_DimDiametric *){
        // do nothing
    }
    virtual void addDimAngular(const DRW_DimAngular *){
        // do nothing
    }
    virtual void addDimAngular3P(const DRW_DimAngular3p *){
        // do nothing
    }
    virtual void addDimOrdinate(const DRW_DimOrdinate *){
        // do nothing
    }
    virtual void addLeader(const DRW_Leader *){
        // do nothing
    }
    virtual void addHatch(const DRW_Hatch *){
        // do nothing
    }
    virtual void addViewport(const DRW_Viewport&){
        // do nothing
    }
    virtual void addImage(const DRW_Image *){
        // do nothing
    }

    virtual void linkImage(const DRW_ImageDef *){
        // do nothing
    }

    virtual void addComment(const char* /*comment*/){}

//writer part, send all in class dx_data to writer
    virtual void writeHeader(DRW_Header& data);
    virtual void writeBlocks();
    //only send the name, needed by the reader to prepare handles of blocks & blockRecords
    virtual void writeBlockRecords();
    //write entities of model space and first paper_space
    virtual void writeEntities();
    virtual void writeLTypes();
    virtual void writeLayers();
    virtual void writeTextstyles();
    virtual void writeVports();
    virtual void writeDimstyles();
    virtual void writeAppId();

    void InitHeader(VarMeasurement varMeasurement, VarInsunits varInsunits);
    void InitLTypes();
    void InitLayers();
    void InitDimstyles();
    void InitTextstyles();

    void AddEntity(DRW_Entity* e);

private:
    dxfRW* dxfW; //pointer to writer, needed to send data
    dx_data cData; // class to store or read data
};

#endif // DX_IFACE_H
